#ifndef COMMAND_ENCODER_H
#define COMMAND_ENCODER_H

#include <cstdint>
#include <vector>

/**
 * @brief STM32 串口命令编码器
 *
 * 将高层运动/动作命令编码为下位机理解的 ASCII 文本字节流。
 *
 * 协议格式（10 字节定长字符串）：
 *   Byte[0]       = 'M'           — 帧头
 *   Byte[1..4]    = cmd           — 4 字符命令名
 *   Byte[5..9]    = data          — 5 位零填充十进制数字
 *
 * 示例：
 *   FORWARD speed=1250  → "Mfwrd01250"
 *   STOP                → "Mstop00000"
 *   GRIP                → "Mfing55555"
 *   WRIST 180°          → "Mfwrs00180"
 *
 * 命令码对应关系（由 send_cmd.py 验证）：
 *   FORWARD         ↔ fwrd    BACKWARD        ↔ bwrd
 *   TURN_LEFT       ↔ ltrn    TURN_RIGHT      ↔ rtrn
 *   TRANSLATE_LEFT  ↔ ltrl    TRANSLATE_RIGHT ↔ rtrl
 *   STOP            ↔ stop    FINGER_GRIP     ↔ fing(55555)
 *   FINGER_RELEASE  ↔ fing(44444)  WRIST_SET  ↔ fwrs
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
        LIFT_UP         = 0x07,
        LIFT_DOWN       = 0x08,
        DOFF_ANGLE      = 0x09,
        DOFS_ANGLE      = 0x0A,
        DOFT_ANGLE      = 0x0B,
        SUCK_VACUUM     = 0x0C,
        SUCK_BLOW       = 0x0D,
        TABLE_UP        = 0x0E,
        TABLE_DOWN      = 0x0F,
        FINGER_GRIP     = 0x10,
        FINGER_RELEASE  = 0x11,
        WRIST_SET       = 0x12,
    };

    /// 将 motion+data 编码为 10 字节 ASCII 命令
    static std::vector<uint8_t> encode(MotionType motion, uint16_t data);

    /// 夹爪夹紧（Mfing55555）
    static std::vector<uint8_t> encodeGrip();

    /// 夹爪松开（Mfing44444）
    static std::vector<uint8_t> encodeRelease();

    /// 腕部角度（Mfwrs + 5 位角度值）
    static std::vector<uint8_t> encodeWristAngle(uint8_t angle_deg);

    /// 停止（Mstop00000）
    static std::vector<uint8_t> encodeStop();

private:
    /// MotionType → 4 字符命令名
    static const char* cmdName(MotionType motion);
};

#endif // COMMAND_ENCODER_H
