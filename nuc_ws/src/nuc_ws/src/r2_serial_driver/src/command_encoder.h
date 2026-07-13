#ifndef COMMAND_ENCODER_H
#define COMMAND_ENCODER_H

#include <cstdint>
#include <vector>

/**
 * @brief STM32 串口命令编码器（与 STM32 parse_motion 严格一致）
 *
 * 将高层运动/动作命令编码为下位机理解的 ASCII 文本字节流。
 *
 * 协议格式（10 字节定长字符串）：
 *   Byte[0]       = 'M'           — 帧头
 *   Byte[1..4]    = cmd           — 4 字符命令名
 *   Byte[5..9]    = data          — 5 位零填充十进制数字
 *
 * 底盘速度映射（由 STM32 固件定义）：
 *   fwrd/bwrd/ltrl/rtrl: data 0~10000 → 0~1.592 m/s
 *   ltrn/rtrn:           data 0~10000 → 0~2.49 rad/s
 *
 * 关节/机构映射：
 *   doff/dofs/doft: data 0~90 → 角度 -45°~+45°（逆时针为正）
 *   fwrs:           data 0~220 → 腕部舵机角度 0°~220°
 *   taup/tadw:      data 为升降距离（mm）
 *
 * 二值命令（data=55555 / data=44444）：
 *   fing 55555=夹紧  44444=松开
 *   suck 55555=吸住  44444=放下
 *   upwd 55555=放物块 44444=爬楼
 *   dwnd 55555=放物块 44444=爬楼
 *
 * 示例：
 *   FORWARD speed=1250  → "Mfwrd01250"
 *   STOP                → "Mstop00000"
 *   GRIP                → "Mfing55555"
 *   WRIST 180°          → "Mfwrs00180"
 */
class CommandEncoder {
public:
    enum MotionType : uint8_t {
        STOP            = 0x00,
        FORWARD         = 0x01,
        BACKWARD        = 0x02,
        TURN_LEFT       = 0x03,
        TURN_RIGHT      = 0x04,
        TRANSLATE_LEFT  = 0x05,
        TRANSLATE_RIGHT = 0x06,
        UP              = 0x07,   // upwd 44444=爬楼 55555=放物块
        DOWN            = 0x08,   // dwnd 44444=爬楼 55555=放物块
        DOFF_ANGLE      = 0x09,
        DOFS_ANGLE      = 0x0A,
        DOFT_ANGLE      = 0x0B,
        SUCK_VACUUM     = 0x0C,   // suck 55555=吸住
        SUCK_BLOW       = 0x0D,   // suck 44444=放下
        TABLE_UP        = 0x0E,   // taup 上升距离(mm)
        TABLE_DOWN      = 0x0F,   // tadw 下降距离(mm)
        FINGER_GRIP     = 0x10,   // fing 55555=夹紧
        FINGER_RELEASE  = 0x11,   // fing 44444=松开
        WRIST_SET       = 0x12,   // fwrs 角度 0~220°
    };

    /// 将 motion+data 编码为 10 字节 ASCII 命令
    static std::vector<uint8_t> encode(MotionType motion, uint16_t data);

    /// 夹爪夹紧（Mfing55555）
    static std::vector<uint8_t> encodeGrip();

    /// 夹爪松开（Mfing44444）
    static std::vector<uint8_t> encodeRelease();

    /// 腕部角度（Mfwrs + 5 位角度值, 0~220°）
    static std::vector<uint8_t> encodeWristAngle(uint8_t angle_deg);

    /// 停止（Mstop00000）
    static std::vector<uint8_t> encodeStop();

    /// 根据帧内容返回 STM32 预期应答字符串（不含结尾换行）
    static const char* expectedAck(const std::vector<uint8_t>& frame);

    /// 上楼/放物块（Mupwd44444 / Mupwd55555）
    static std::vector<uint8_t> encodeUpClimb();
    static std::vector<uint8_t> encodeUpPlace();

    /// 下楼/放物块（Mdwnd44444 / Mdwnd55555）
    static std::vector<uint8_t> encodeDownClimb();
    static std::vector<uint8_t> encodeDownPlace();

    /// 吸盘吸住/放下（Msuck55555 / Msuck44444）
    static std::vector<uint8_t> encodeSuckVacuum();
    static std::vector<uint8_t> encodeSuckBlow();

private:
    /// MotionType → 4 字符命令名
    static const char* cmdName(MotionType motion);
};

#endif // COMMAND_ENCODER_H
