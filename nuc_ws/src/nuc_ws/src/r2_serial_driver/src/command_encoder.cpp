#include "command_encoder.h"

#include <cstdio>
#include <string>

// MotionType → 4 字符 ASCII 命令名
const char* CommandEncoder::cmdName(MotionType motion) {
    switch (motion) {
        case STOP:            return "stop";
        case FORWARD:         return "fwrd";   // data 0~10000 → 0~1.592 m/s
        case BACKWARD:        return "bwrd";
        case TURN_LEFT:       return "ltrn";   // data 0~10000 → 0~2.49 rad/s
        case TURN_RIGHT:      return "rtrn";
        case TRANSLATE_LEFT:  return "ltrl";
        case TRANSLATE_RIGHT: return "rtrl";
        case UP:              return "upwd";   // 55555=放物块 44444=爬楼
        case DOWN:            return "dwnd";
        case DOFF_ANGLE:      return "doff";   // data 0~90 → -45°~+45°
        case DOFS_ANGLE:      return "dofs";
        case DOFT_ANGLE:      return "doft";
        case SUCK_VACUUM:     return "suck";   // data=55555 吸住
        case SUCK_BLOW:       return "suck";   // data=44444 放下
        case TABLE_UP:        return "taup";   // data=上升距离(mm)
        case TABLE_DOWN:      return "tadw";
        case FINGER_GRIP:     return "fing";   // data=55555 夹紧
        case FINGER_RELEASE:  return "fing";   // data=44444 松开
        case WRIST_SET:       return "fwrs";   // data 0~220°
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

std::vector<uint8_t> CommandEncoder::encodeUpClimb() {
    return encode(UP, 44444);
}

std::vector<uint8_t> CommandEncoder::encodeUpPlace() {
    return encode(UP, 55555);
}

std::vector<uint8_t> CommandEncoder::encodeDownClimb() {
    return encode(DOWN, 44444);
}

std::vector<uint8_t> CommandEncoder::encodeDownPlace() {
    return encode(DOWN, 55555);
}

std::vector<uint8_t> CommandEncoder::encodeSuckVacuum() {
    return encode(SUCK_VACUUM, 55555);
}

std::vector<uint8_t> CommandEncoder::encodeSuckBlow() {
    return encode(SUCK_BLOW, 44444);
}

const char* CommandEncoder::expectedAck(const std::vector<uint8_t>& frame) {
    if (frame.size() < 10) return nullptr;

    std::string cmd(frame.begin() + 1, frame.begin() + 5);
    std::string data(frame.begin() + 5, frame.begin() + 10);

    if (cmd == "stop") return "Car stopped";
    if (cmd == "fwrd") return "Car forward";
    if (cmd == "bwrd") return "Car backward";
    if (cmd == "ltrn") return "Car turn left";
    if (cmd == "rtrn") return "Car turn right";
    if (cmd == "ltrl") return "Car translate left";
    if (cmd == "rtrl") return "Car translate right";

    if (cmd == "upwd") return (data == "55555") ? "car block up" : "car lift up";
    if (cmd == "dwnd") return (data == "55555") ? "car block down" : "car lift down";

    if (cmd == "doff") return "doff set";
    if (cmd == "dofs") return "dofs set";
    if (cmd == "doft") return "doft set";

    if (cmd == "suck") return (data == "55555") ? "suck vacuum" : "suck blow";

    if (cmd == "taup") return "table up";
    if (cmd == "tadw") return "table down";

    if (cmd == "fing") return (data == "55555") ? "finger grip" : "finger release";
    if (cmd == "fwrs") return "finger wrist set";

    return nullptr;
}
