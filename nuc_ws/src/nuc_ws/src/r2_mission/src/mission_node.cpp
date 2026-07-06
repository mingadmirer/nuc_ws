#include "mission_node.h"

#include <chrono>
#include <cmath>
#include <algorithm>
#include <string>
#include <thread>

using namespace std::chrono_literals;

// ── 析构 ──
MissionNode::~MissionNode() { serial_.close(); }

// ── 构造 ──
MissionNode::MissionNode(const rclcpp::NodeOptions& opts)
    : Node("r2_mission", opts)
{
    // ── 开环运动：速度 + 时长 ──
    initial_right_speed_     = declare_parameter("initial_right_speed", 2000);
    initial_right_time_      = declare_parameter("initial_right_time_s", 3.0);
    translate_left_speed_    = declare_parameter("translate_left_speed", 1250);
    translate_left_time_s_   = declare_parameter("translate_left_time_s", 1.5);
    rotate_right_speed_      = declare_parameter("rotate_right_speed", 2000);
    rotate_right_time_s_     = declare_parameter("rotate_right_time_s", 2.0);

    // ── 时序 ──
    startup_delay_s_ = declare_parameter("startup_delay_s", 5.0);

    // ── 批次伺服 ──
    det_window_size_       = declare_parameter("det_window_size", 30);
    det_min_inliers_       = declare_parameter("det_min_inliers", 20);
    det_consistency_pct_   = declare_parameter("det_consistency_pct", 0.2);
    servo_move_duration_s_ = declare_parameter("servo_move_duration_s", 1.0);

    // ── 深度伺服 ──
    target_depth_mm_    = declare_parameter("target_depth_mm", 370.0);
    depth_threshold_mm_ = declare_parameter("depth_threshold_mm", 20.0);
    kp_                 = declare_parameter("kp", 0.5);
    kd_                 = declare_parameter("kd", 0.0);
    max_servo_speed_    = declare_parameter("max_servo_speed", 1000);
    min_servo_speed_    = declare_parameter("min_servo_speed", 500);

    // ── 横向伺服（相机内参 + 控制参数）──
    fx_                   = declare_parameter("fx", 415.9);
    cx_                   = declare_parameter("cx", 316.6);
    img_width_            = declare_parameter("img_width", 640);
    lateral_kp_           = declare_parameter("lateral_kp", 0.5);
    lateral_kd_           = declare_parameter("lateral_kd", 0.0);
    lateral_threshold_mm_ = declare_parameter("lateral_threshold_mm", 15.0);
    lateral_max_speed_    = declare_parameter("lateral_max_speed", 1250);
    lateral_min_speed_    = declare_parameter("lateral_min_speed", 1000);
    lateral_timeout_s_    = declare_parameter("lateral_timeout_s", 10.0);
    // ── 最终盲走近端 ──
    final_approach_speed_ = declare_parameter("final_approach_speed", 800);
    final_approach_time_s_= declare_parameter("final_approach_time_s", 0.6);

    // ── 夹爪 ──
    grip_delay_s_     = declare_parameter("grip_delay_s", 0.5);
    wrist_angle_deg_  = declare_parameter("wrist_angle_deg", 210);
    wrist_delay_s_    = declare_parameter("wrist_delay_s", 0.5);
    release_delay_s_  = declare_parameter("release_delay_s", 5.0);

    // ── 串口：轮询等待 STM32 上电并握手 ──
    std::string dev = declare_parameter("serial_device", "/dev/serial/by-id/usb-STMicroelectronics_STM32_Virtual_ComPort_326B326C3034-if00");
    int baud = declare_parameter("baudrate", 115200);

    RCLCPP_INFO(get_logger(), "等待 STM32 上电: %s", dev.c_str());

    bool ready = false;
    int retry = 0;
    const uint8_t hs_cmd[] = {'M','s','t','o','p','0','0','0','0','0'};

    while (!ready && rclcpp::ok()) {
        ++retry;

        // 1. 尝试打开串口
        if (!serial_.isOpen()) {
            if (!serial_.open(dev, baud)) {
                RCLCPP_WARN(get_logger(), "[第%d次] 串口未就绪，1秒后重试...", retry);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
            RCLCPP_INFO(get_logger(), "[第%d次] 串口已打开", retry);
        }

        // 2. 握手: Mstop00000 → 期待 Car stopped
        serial_.flush();
        serial_.send(hs_cmd, 10);

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        uint8_t buf[64] = {};
        int n = serial_.recv(buf, sizeof(buf) - 1, 200);
        if (n > 0) {
            std::string resp(reinterpret_cast<char*>(buf), n);
            RCLCPP_INFO(get_logger(), "[第%d次] 握手返回: %s", retry, resp.c_str());
            if (resp.find("Car stopped") != std::string::npos) {
                ready = true;
                RCLCPP_INFO(get_logger(), "✓ STM32 固件就绪！（%d次握手成功）", retry);
            } else {
                RCLCPP_WARN(get_logger(), "[第%d次] 返回异常，重试...", retry);
            }
        } else {
            RCLCPP_WARN(get_logger(), "[第%d次] 无应答，重试...", retry);
        }

        if (!ready) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    // 如果 rclcpp::ok() 为 false，节点正在关闭
    if (!ready) {
        RCLCPP_ERROR(get_logger(), "✗ STM32 握手失败，节点继续运行但串口不可用！");
    }

    // ── 订阅 /detections（RDK 发布，NTP 时间同步）──
    sub_det_ = create_subscription<detection_interfaces::msg::DetectionArray>(
        "/detections", 10,
        std::bind(&MissionNode::onDetection, this, std::placeholders::_1));

    // ── 主循环 10Hz ──
    tick_timer_ = create_wall_timer(100ms, std::bind(&MissionNode::onTick, this));

    RCLCPP_INFO(get_logger(), "=== 崇武探幽 v1 启动 ===");
    sendWristAngle(120);  // 夹爪复位到120
    transitionTo(INIT);
}

// ── 状态名 ──
std::string MissionNode::stateName(State s) {
    switch (s) {
        case INIT:            return "INIT";
        case ALIGN_RACK:      return "ALIGN_RACK";
        case APPROACH_SPEAR:  return "APPROACH_SPEAR";
        case ALIGN_LATERAL:   return "ALIGN_LATERAL";
        case FINAL_APPROACH:  return "FINAL_APPROACH";
        case GRIP_LIFT:       return "GRIP_LIFT";
        case RETREAT_ROTATE:  return "RETREAT_ROTATE";
        case COMPLETE:        return "COMPLETE";
        default:              return "ERROR";
    }
}

// ── 状态转移 ──
void MissionNode::transitionTo(State s) {
    RCLCPP_INFO(get_logger(), "[%s] → [%s]", stateName(state_).c_str(), stateName(s).c_str());
    state_ = s;
    state_enter_ = now();
    if (delay_timer_)       { delay_timer_->cancel();       delay_timer_.reset(); }
    if (servo_move_timer_)  { servo_move_timer_->cancel();  servo_move_timer_.reset(); }
    servo_buffer_.clear();
    servo_frame_count_ = 0;
    servo_moving_ = false;

    switch (s) {
    case INIT: {
        // 等待启动缓冲让操作员就位
        RCLCPP_INFO(get_logger(), "等待 %.0fs 缓冲...", startup_delay_s_);
        delay_timer_ = create_wall_timer(
            std::chrono::duration<double>(startup_delay_s_),
        [this]() {
            delay_timer_->cancel();
            transitionTo(ALIGN_RACK);
        });
        break;
    }

    case ALIGN_RACK: {
        RCLCPP_INFO(get_logger(), "Step0: 初始右移 speed=%d time=%.1fs",
                    initial_right_speed_, initial_right_time_);
        sendFrame(CommandEncoder::encode(CommandEncoder::TRANSLATE_RIGHT,
                                         initial_right_speed_));

        delay_timer_ = create_wall_timer(
            std::chrono::duration<double>(initial_right_time_ + 0.3),
        [this]() {
            delay_timer_->cancel();
            sendFrame(CommandEncoder::encodeStop());
            transitionTo(APPROACH_SPEAR);
        });
        break;
    }

    case APPROACH_SPEAR: {
        RCLCPP_INFO(get_logger(), "Step3: 深度伺服 → %.0fmm (kp=%.1f kd=%.1f)",
                    target_depth_mm_, kp_, kd_);
        break;
    }

    case ALIGN_LATERAL: {
        RCLCPP_INFO(get_logger(), "Step3b: 横向伺服 (kp=%.1f kd=%.1f threshold=%.0fmm)",
                    lateral_kp_, lateral_kd_, lateral_threshold_mm_);
        break;
    }

    case FINAL_APPROACH: {
        RCLCPP_INFO(get_logger(), "Step3c: 盲走近端 speed=%d time=%.1fs (无需视觉)",
                    final_approach_speed_, final_approach_time_s_);
        sendFrame(CommandEncoder::encode(CommandEncoder::TRANSLATE_RIGHT,
                                         final_approach_speed_));
        delay_timer_ = create_wall_timer(
            std::chrono::duration<double>(final_approach_time_s_ + 0.1),
        [this]() {
            delay_timer_->cancel();
            sendFrame(CommandEncoder::encodeStop());
            // 腕部降到100，再右移800×0.5s
            sendWristAngle(100);
            RCLCPP_INFO(get_logger(), "腕部降至100，右移盲走800×1.0s");
            sendFrame(CommandEncoder::encode(CommandEncoder::TRANSLATE_RIGHT, 800));
            delay_timer_ = create_wall_timer(1.1s, [this]() {
                delay_timer_->cancel();
                sendFrame(CommandEncoder::encodeStop());
                transitionTo(GRIP_LIFT);
            });
        });
        break;
    }

    case GRIP_LIFT: {
        RCLCPP_INFO(get_logger(), "Step4: 夹取");
        sendGrip();
        delay_timer_ = create_wall_timer(
            std::chrono::duration<double>(grip_delay_s_),
        [this]() {
            delay_timer_->cancel();
            RCLCPP_INFO(get_logger(), "Step5: 腕部上旋 210°");
            sendWristAngle(210);
            delay_timer_ = create_wall_timer(
                std::chrono::duration<double>(wrist_delay_s_),
            [this]() {
                delay_timer_->cancel();
                transitionTo(RETREAT_ROTATE);
            });
        });
        break;
    }

    case RETREAT_ROTATE: {
        RCLCPP_INFO(get_logger(), "Step6: 左移(退开) speed=%d time=%.1fs",
                    translate_left_speed_, translate_left_time_s_);
        sendFrame(CommandEncoder::encode(CommandEncoder::TRANSLATE_LEFT,
                                         translate_left_speed_));

        delay_timer_ = create_wall_timer(
            std::chrono::duration<double>(translate_left_time_s_ + 0.3),
        [this]() {
            delay_timer_->cancel();
            RCLCPP_INFO(get_logger(), "Step7: 右旋 speed=%d time=%.1fs",
                        rotate_right_speed_, rotate_right_time_s_);
            sendFrame(CommandEncoder::encode(CommandEncoder::TURN_RIGHT,
                                             rotate_right_speed_));

            delay_timer_ = create_wall_timer(
                std::chrono::duration<double>(rotate_right_time_s_ + 0.5),
            [this]() {
                delay_timer_->cancel();
                sendFrame(CommandEncoder::encodeStop());
                RCLCPP_INFO(get_logger(), "全车停止");
                transitionTo(COMPLETE);
            });
        });
        break;
    }

    case COMPLETE: {
        RCLCPP_INFO(get_logger(), "=== 崇武探幽 完成 ===");
        RCLCPP_INFO(get_logger(), "等待 %.0fs 后松开夹爪...", release_delay_s_);
        delay_timer_ = create_wall_timer(
            std::chrono::duration<double>(release_delay_s_),
        [this]() {
            delay_timer_->cancel();
            RCLCPP_INFO(get_logger(), "松开夹爪");
            sendRelease();
        });
        break;
    }

    default: break;
    }
}

// ── 主循环 tick（10Hz）──
void MissionNode::onTick() {
    auto dt = (now() - state_enter_).seconds();

    if (state_ == APPROACH_SPEAR && dt > 15.0) {
        RCLCPP_WARN(get_logger(), "[%s] 深度伺服超时 %.0fs，强制推进",
                    stateName(state_).c_str(), dt);
        sendFrame(CommandEncoder::encodeStop());
        transitionTo(ALIGN_LATERAL);
    } else if (state_ == ALIGN_LATERAL && dt > lateral_timeout_s_) {
        RCLCPP_WARN(get_logger(), "[%s] 横向伺服超时 %.0fs，强制推进",
                    stateName(state_).c_str(), dt);
        sendFrame(CommandEncoder::encodeStop());
        transitionTo(FINAL_APPROACH);
    }
}

// ── 检测回调（连续N帧→一致性筛选→锁定目标→一次移动→重复）──
void MissionNode::onDetection(const detection_interfaces::msg::DetectionArray::SharedPtr msg) {
    if (state_ != APPROACH_SPEAR && state_ != ALIGN_LATERAL) return;
    if (servo_moving_) return;  // 移动中，不收集

    // 每收到一帧 /detections 就计数（不管有没有 lance）
    servo_frame_count_++;

    for (const auto& d : msg->detections) {
        if (d.name != "lance") continue;
        if (d.depth_mm <= 0.0f) {
            RCLCPP_WARN(get_logger(), "[%s] 第%d帧 无效深度: %.0fmm",
                        stateName(state_).c_str(), servo_frame_count_, d.depth_mm);
            continue;
        }

        DetSample s;
        s.depth_mm = d.depth_mm;
        s.cx_norm  = (d.x1 + d.x2) * 0.5f;
        s.cy_norm  = (d.y1 + d.y2) * 0.5f;
        servo_buffer_.push_back(s);

        RCLCPP_INFO(get_logger(), "[%s] 第%d帧 检测: lance 深度=%.0fmm 像素=(%.0f,%.0f) 有效帧=%zu/%d",
                    stateName(state_).c_str(), servo_frame_count_,
                    d.depth_mm, s.cx_norm * img_width_, s.cy_norm * 400.0f,
                    servo_buffer_.size(), det_window_size_);
        break;  // 一帧只取第一个 lance
    }

    // 无论本帧有没有 lance，连续 30 帧后做一致性检查
    if (servo_frame_count_ >= det_window_size_) {
        int total_lance = static_cast<int>(servo_buffer_.size());

        RCLCPP_INFO(get_logger(),
            "[%s] 窗口满: 连续%d帧中 %d帧有lance",
            stateName(state_).c_str(), servo_frame_count_, total_lance);

        if (total_lance < det_min_inliers_) {
            // lance 帧本身就不够 20，丢弃最早 10 个计数，继续
            RCLCPP_WARN(get_logger(),
                "[%s] lance帧不足(%d<%d)，丢弃10帧继续",
                stateName(state_).c_str(), total_lance, det_min_inliers_);
            servo_frame_count_ -= 10;
            if (servo_frame_count_ < 0) servo_frame_count_ = 0;
            // 也清理掉对应时间段的有效帧
            int to_drop = std::min(10, total_lance);
            servo_buffer_.erase(servo_buffer_.begin(),
                                servo_buffer_.begin() + to_drop);
            return;
        }

        // 中位数
        std::vector<float> depths, cxs;
        for (auto& sd : servo_buffer_) {
            depths.push_back(sd.depth_mm);
            cxs.push_back(sd.cx_norm * static_cast<float>(img_width_));
        }
        std::sort(depths.begin(), depths.end());
        std::sort(cxs.begin(), cxs.end());
        float med_d = depths[depths.size() / 2];
        float med_c = cxs[cxs.size() / 2];

        // 一致性筛选（偏离中位数 < 20%）
        float sum_d = 0, sum_cx = 0;
        int inliers = 0;
        for (auto& sd : servo_buffer_) {
            float dd = std::abs(sd.depth_mm - med_d) / std::max(med_d, 1.0f);
            float dp = std::abs(sd.cx_norm * img_width_ - med_c) / std::max(med_c, 1.0f);
            if (dd <= det_consistency_pct_ && dp <= det_consistency_pct_) {
                sum_d  += sd.depth_mm;
                sum_cx += sd.cx_norm * img_width_;
                ++inliers;
            }
        }

        RCLCPP_INFO(get_logger(),
            "[%s] 一致性: 深度中位数=%.0f 像素中位数=%.0f 一致帧=%d/%d (需>=%d)",
            stateName(state_).c_str(), med_d, med_c, inliers, total_lance, det_min_inliers_);

        if (inliers >= det_min_inliers_) {
            float avg_d  = sum_d / static_cast<float>(inliers);
            float avg_cx = sum_cx / static_cast<float>(inliers);
            float pixel_off = avg_cx - static_cast<float>(cx_);

            RCLCPP_INFO(get_logger(),
                "[%s] ★ 目标锁定: 深度=%.0fmm 像素中心=%.0fpx",
                stateName(state_).c_str(), avg_d, avg_cx);

            double move_time_s = servo_move_duration_s_;

            if (state_ == APPROACH_SPEAR) {
                float err = avg_d - static_cast<float>(target_depth_mm_);
                if (std::abs(err) < depth_threshold_mm_) {
                    RCLCPP_INFO(get_logger(), "[%s] 深度到达！", stateName(state_).c_str());
                    sendFrame(CommandEncoder::encodeStop());
                    transitionTo(ALIGN_LATERAL);
                    return;
                }
                // 速度2000≈250mm/s，时间=距离/速度
                move_time_s = std::abs(err) / 250.0;
                move_time_s = std::clamp(move_time_s, 0.1, 3.0);
                RCLCPP_INFO(get_logger(), "[%s] 深度偏差=%.0fmm 移动=%.2fs",
                            stateName(state_).c_str(), err, move_time_s);
                if (err > 0)
                    sendFrame(CommandEncoder::encode(CommandEncoder::TRANSLATE_RIGHT, 2000));
                else
                    sendFrame(CommandEncoder::encode(CommandEncoder::TRANSLATE_LEFT, 2000));
            } else {
                float lateral_err = -pixel_off * avg_d / static_cast<float>(fx_);
                if (std::abs(lateral_err) < lateral_threshold_mm_) {
                    RCLCPP_INFO(get_logger(), "[%s] 横向到达！", stateName(state_).c_str());
                    sendFrame(CommandEncoder::encodeStop());
                    transitionTo(FINAL_APPROACH);
                    return;
                }
                // 速度2000≈300mm/s，时间=距离/速度
                move_time_s = std::abs(lateral_err) / 300.0;
                move_time_s = std::clamp(move_time_s, 0.1, 3.0);
                RCLCPP_INFO(get_logger(), "[%s] 横向偏差=%.0fmm 移动=%.2fs",
                            stateName(state_).c_str(), lateral_err, move_time_s);
                if (lateral_err > 0)
                    sendFrame(CommandEncoder::encode(CommandEncoder::FORWARD, 2000));
                else
                    sendFrame(CommandEncoder::encode(CommandEncoder::BACKWARD, 2000));
            }

            servo_buffer_.clear();
            servo_frame_count_ = 0;
            servo_moving_ = true;
            if (servo_move_timer_) servo_move_timer_->cancel();
            servo_move_timer_ = create_wall_timer(
                std::chrono::duration<double>(move_time_s),
            [this]() {
                servo_move_timer_->cancel();
                sendFrame(CommandEncoder::encodeStop());
                servo_moving_ = false;
                RCLCPP_INFO(get_logger(), "[%s] 移动结束，重新收集",
                            stateName(state_).c_str());
            });
        } else {
            RCLCPP_WARN(get_logger(),
                "[%s] 一致帧不足(%d<%d)，丢弃10帧继续",
                stateName(state_).c_str(), inliers, det_min_inliers_);
            servo_frame_count_ -= 10;
            if (servo_frame_count_ < 0) servo_frame_count_ = 0;
            servo_buffer_.erase(servo_buffer_.begin(),
                                servo_buffer_.begin() + std::min(10, total_lance));
        }
    }
}

// ── 夹爪 ──
void MissionNode::sendGrip()       { sendFrame(CommandEncoder::encodeGrip()); }
void MissionNode::sendRelease()    { sendFrame(CommandEncoder::encodeRelease()); }
void MissionNode::sendWristAngle(int deg) {
    sendFrame(CommandEncoder::encodeWristAngle(static_cast<uint8_t>(deg)));
}

// ── 命令名称映射 ──
static const char* motionName(const std::vector<uint8_t>& frame) {
    if (frame.size() < 6) return "?";
    // frame: M + 4-char cmd + 5-digit data  (10 bytes ASCII)
    std::string cmd(frame.begin() + 1, frame.begin() + 5);
    if (cmd == "fwrd") return "前进";
    if (cmd == "bwrd") return "后退";
    if (cmd == "ltrn") return "左转";
    if (cmd == "rtrn") return "右转";
    if (cmd == "ltrl") return "左移";
    if (cmd == "rtrl") return "右移";
    if (cmd == "stop") return "停止";
    if (cmd == "fing") {
        std::string data(frame.begin() + 5, frame.begin() + 10);
        return (data == "55555") ? "夹爪夹紧" : (data == "44444") ? "夹爪松开" : "夹爪";
    }
    if (cmd == "fwrs") return "腕部";
    return "?";
}

static int motionData(const std::vector<uint8_t>& frame) {
    if (frame.size() < 10) return 0;
    return std::stoi(std::string(frame.begin() + 5, frame.begin() + 10));
}

// ── 串口 ──
bool MissionNode::sendFrame(const std::vector<uint8_t>& frame) {
    if (!serial_.isOpen()) {
        RCLCPP_ERROR(get_logger(), "串口未打开，无法发送");
        return false;
    }
    bool ok = serial_.send(frame.data(), frame.size());
    if (!ok) {
        RCLCPP_ERROR(get_logger(), "串口写入失败 (len=%zu)", frame.size());
    } else {
        RCLCPP_INFO(get_logger(), "[%s] 串口发送: %s %d",
                    stateName(state_).c_str(),
                    motionName(frame), motionData(frame));
    }
    return ok;
}

// ── main ──
int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MissionNode>());
    rclcpp::shutdown();
    return 0;
}
