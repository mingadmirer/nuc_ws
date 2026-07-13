#ifndef MISSION_NODE_H
#define MISSION_NODE_H

#include <rclcpp/rclcpp.hpp>
#include <detection_interfaces/msg/detection_array.hpp>

#include "serial_interface.h"
#include "command_encoder.h"

/**
 * 崇武探幽 v1 单体节点（红队）
 *
 * 集串口驱动、检测订阅、状态机于一体。
 *
 * 流程：INIT → ALIGN_RACK → APPROACH_SPEAR(深度伺服)
 *       → ALIGN_LATERAL(横向伺服) → FINAL_APPROACH(盲走近端)
 *       → GRIP_LIFT → RETREAT_ROTATE → COMPLETE
 */
class MissionNode : public rclcpp::Node {
public:
    enum State {
        INIT,
        ALIGN_RACK,
        APPROACH_SPEAR,
        ALIGN_LATERAL,
        FINAL_APPROACH,
        GRIP_LIFT,
        RETREAT_ROTATE,
        COMPLETE,
        RECOVERY,
        ERROR
    };

    explicit MissionNode(const rclcpp::NodeOptions& opts = rclcpp::NodeOptions());
    ~MissionNode() override;

private:
    // 状态机
    void transitionTo(State s);
    void onTick();
    void onDetection(const detection_interfaces::msg::DetectionArray::SharedPtr msg);

    // 动作
    void sendGrip();
    void sendRelease();
    void sendWristAngle(int deg);

    // 串口（委托 serial_.sendReliable）
    bool sendFrameReliable(const std::vector<uint8_t>& frame);
    bool handshakeSTM32();

    // 工具
    static std::string stateName(State s);

    // ── 检测缓存（批次伺服）──
    struct DetSample {
        float depth_mm;
        float cx_norm;    // bbox 中心 x (归一化 0~1)
        float cy_norm;    // bbox 中心 y (归一化 0~1)
    };
    std::vector<DetSample> servo_buffer_;     // 收集有效 lance 帧
    int servo_frame_count_ = 0;               // 连续 /detections 帧计数（含无效）
    bool servo_moving_ = false;

    // ── 状态 ──
    State state_ = INIT;
    rclcpp::Time state_enter_;
    rclcpp::TimerBase::SharedPtr tick_timer_;      // 10Hz 主循环（超时检测）
    rclcpp::TimerBase::SharedPtr delay_timer_;     // 状态内延时/排序定时器
    rclcpp::TimerBase::SharedPtr servo_move_timer_; // 伺服批次移动定时器

    // ── 订阅 ──
    rclcpp::Subscription<detection_interfaces::msg::DetectionArray>::SharedPtr sub_det_;

    // ── 串口 ──
    SerialInterface serial_;
    std::string serial_device_;
    int baudrate_;
    int consecutive_failures_ = 0;

    // ── 参数 ──
    // 批次伺服
    int det_window_size_;
    int det_min_inliers_;
    double det_consistency_pct_;
    double servo_move_duration_s_;
    // 开环运动：速度(0~10000) + 时长(s)
    int initial_right_speed_;
    double initial_right_time_;
    int translate_left_speed_;
    double translate_left_time_s_;
    int rotate_right_speed_;
    double rotate_right_time_s_;

    // 时序
    double startup_delay_s_;

    // 深度伺服
    double target_depth_mm_;
    double depth_threshold_mm_;
    double kp_;
    double kd_;
    int max_servo_speed_;
    int min_servo_speed_;

    // 横向伺服（相机内参 + 控制参数）
    double fx_;
    double cx_;
    int img_width_;
    double lateral_kp_;
    double lateral_kd_;
    double lateral_threshold_mm_;
    int lateral_max_speed_;
    int lateral_min_speed_;
    double lateral_timeout_s_;

    // 最终盲走近端
    int final_approach_speed_;
    double final_approach_time_s_;

    // 夹爪
    double grip_delay_s_;
    int wrist_angle_deg_;
    double wrist_delay_s_;
    double release_delay_s_;
};

#endif // MISSION_NODE_H
