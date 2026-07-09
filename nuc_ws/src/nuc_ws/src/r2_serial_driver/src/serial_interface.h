#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <string>
#include <cstdint>

/**
 * @brief 底层串口封装（基于 POSIX termios）
 *
 * 负责：
 * - 打开/关闭 USB 虚拟串口设备（如 /dev/ttyACM0）
 * - 设置波特率、数据位、停止位、校验位
 * - 发送字节流
 * - 接收并缓存应答数据
 */
class SerialInterface {
public:
    SerialInterface();
    ~SerialInterface();

    /// 打开串口设备 @param device 如 "/dev/ttyACM0"  @param baud 如 115200
    bool open(const std::string& device, int baud = 115200);

    /// 关闭串口
    void close();

    /// 是否已打开
    bool isOpen() const { return fd_ >= 0; }

    /// 发送 len 字节（阻塞）
    bool send(const uint8_t* data, size_t len);

    /// 可靠发送：发送后等待 expected_ack，收不到或内容不匹配则重试（最多 max_retries 次）
    bool sendReliable(const uint8_t* data, size_t len,
                      const char* expected_ack, int max_retries = 5);

    /// 接收，最多读 max_len 字节，返回实际读到的字节数（非阻塞）
    int recv(uint8_t* buf, size_t max_len, int timeout_ms = 100);

    /// 清空收发缓冲区
    void flush();

private:
    int fd_;                         ///< 串口文件描述符
    std::string device_;             ///< 设备路径
    int baudrate_;                   ///< 波特率

    /// POSIX baudrate 常量转换
    static int baudToConstant(int baud);
};

#endif // SERIAL_INTERFACE_H
