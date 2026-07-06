#include "serial_interface.h"

#include <cstring>
#include <stdexcept>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

SerialInterface::SerialInterface() : fd_(-1), baudrate_(115200) {}

SerialInterface::~SerialInterface() { close(); }

bool SerialInterface::open(const std::string& device, int baud) {
    if (isOpen()) close();

    device_ = device;
    baudrate_ = baud;

    // O_RDWR | O_NOCTTY: 读写模式，不将此 fd 设为控制终端
    fd_ = ::open(device.c_str(), O_RDWR | O_NOCTTY);
    if (fd_ < 0) {
        return false;
    }

    struct termios tio;
    memset(&tio, 0, sizeof(tio));

    // 控制模式标志
    tio.c_cflag = baudToConstant(baud) | CS8 | CLOCAL | CREAD;
    // CS8   — 8 位数据位
    // CLOCAL— 不监控调制解调器状态
    // CREAD — 使能接收

    // 无校验
    tio.c_cflag &= ~PARENB;
    // 1 位停止位
    tio.c_cflag &= ~CSTOPB;

    // 本地模式：关闭所有本地处理
    tio.c_lflag = 0;
    // 输入模式：原始数据，不做特殊处理
    tio.c_iflag = 0;
    // 输出模式：原始输出
    tio.c_oflag = 0;

    // VTIME=1, VMIN=0: 非阻塞模式，read() 最多等 100ms
    tio.c_cc[VTIME] = 1;
    tio.c_cc[VMIN] = 0;

    // 设置流控（无硬件流控）
    tio.c_cflag &= ~CRTSCTS;

    // 立即应用设置
    if (tcsetattr(fd_, TCSANOW, &tio) < 0) {
        ::close(fd_);
        fd_ = -1;
        return false;
    }

    // 清空缓冲区
    tcflush(fd_, TCIOFLUSH);

    return true;
}

void SerialInterface::close() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

bool SerialInterface::send(const uint8_t* data, size_t len) {
    if (!isOpen()) return false;
    ssize_t written = ::write(fd_, data, len);
    return written == static_cast<ssize_t>(len);
}

int SerialInterface::recv(uint8_t* buf, size_t max_len, int timeout_ms) {
    if (!isOpen() || max_len == 0) return 0;

    struct timeval tv;
    tv.tv_sec  = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd_, &set);

    int ret = select(fd_ + 1, &set, nullptr, nullptr, &tv);
    if (ret <= 0) return 0;  // 超时或出错

    ssize_t n = ::read(fd_, buf, max_len);
    return (n < 0) ? 0 : static_cast<int>(n);
}

void SerialInterface::flush() {
    if (isOpen()) {
        tcflush(fd_, TCIOFLUSH);
    }
}

int SerialInterface::baudToConstant(int baud) {
    switch (baud) {
        case 9600:   return B9600;
        case 19200:  return B19200;
        case 38400:  return B38400;
        case 57600:  return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 921600: return B921600;
        default:     return B115200;
    }
}
