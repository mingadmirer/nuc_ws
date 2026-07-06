#include "command_encoder.h"

#include <cstdio>

// MotionType → 4 字符 ASCII 命令名
const char* CommandEncoder::cmdName(MotionType motion) {
    switch (motion) {
        case STOP:            return "stop";
        case FORWARD:         return "fwrd";
        case BACKWARD:        return "bwrd";
        case TURN_LEFT:       return "ltrn";
        case TURN_RIGHT:      return "rtrn";
        case TRANSLATE_LEFT:  return "ltrl";
        case TRANSLATE_RIGHT: return "rtrl";
        case LIFT_UP:         return "lupt";   // 未在 send_cmd.py 验证
        case LIFT_DOWN:       return "ldwn";
        case DOFF_ANGLE:      return "doff";
        case DOFS_ANGLE:      return "dofs";
        case DOFT_ANGLE:      return "doft";
        case SUCK_VACUUM:     return "suck";   // data=55555
        case SUCK_BLOW:       return "suck";   // data=44444
        case TABLE_UP:        return "tblu";
        case TABLE_DOWN:      return "tbld";
        case FINGER_GRIP:     return "fing";   // data=55555
        case FINGER_RELEASE:  return "fing";   // data=44444
        case WRIST_SET:       return "fwrs";
        default:              return "stop";
    }
}

std::vector<uint8_t> CommandEncoder::encode(MotionType motion, uint16_t data) {
    // 格式: M + 4字符命令 + 5位十进制数字 = 10 字节
    char buf[11];
    snprintf(buf, sizeof(buf), "M%s%05u", cmdName(motion), data);
    return std::vector<uint8_t>(buf, buf + 10);
}

std::vector<uint8_t> CommandEncoder::encodeGrip() {
    return encode(FINGER_GRIP, 55555);
}

std::vector<uint8_t> CommandEncoder::encodeRelease() {
    return encode(FINGER_RELEASE, 44444);
}

std::vector<uint8_t> CommandEncoder::encodeWristAngle(uint8_t angle_deg) {
    if (angle_deg > 220) angle_deg = 220;
    return encode(WRIST_SET, angle_deg);
}

std::vector<uint8_t> CommandEncoder::encodeStop() {
    return encode(STOP, 0);
}
