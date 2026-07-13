#!/usr/bin/env python3
"""
监听 /dev/ttyACM* 串口，等待并打印 STM32 发来的消息。
用法: python3 monitor_serial.py [设备路径]
      - 不传参数：自动扫描 /dev/ttyACM* 并选择第一个
      - 传参数：使用指定设备，如 python3 monitor_serial.py /dev/ttyACM0
"""

import serial
import sys
import glob
import time
from datetime import datetime


BAUDRATE = 115200
TIMEOUT = 1  # 读超时秒数

# 需要高亮的关键词列表
HIGHLIGHT_KEYWORDS = [
    "CHASSIS_ON",
    "CHASSIS_OFF",
    "MOTOR_ON",
    "MOTOR_OFF",
    "EMERGENCY",
    "ERROR",
    "READY",
    "DONE",
    "OK",
]


def find_acm_devices():
    """扫描 /dev/ttyACM* 设备，按名称排序返回。"""
    devices = sorted(glob.glob("/dev/ttyACM*"))
    return devices


def main():
    # 确定串口设备
    if len(sys.argv) > 1:
        device = sys.argv[1]
    else:
        devices = find_acm_devices()
        if not devices:
            print("[错误] 未找到 /dev/ttyACM* 设备，请检查连接。", file=sys.stderr)
            sys.exit(1)
        device = devices[0]
        if len(devices) > 1:
            print(f"[信息] 发现多个设备: {devices}，使用第一个: {device}")

    print(f"[监听] 串口: {device}  波特率: {BAUDRATE}")
    print(f"[监听] 等待数据... (Ctrl+C 退出)\n")

    while True:
        try:
            ser = serial.Serial(device, BAUDRATE, timeout=TIMEOUT)
            ser.reset_input_buffer()
            print(f"[连接] {device} 已打开")
            break
        except (serial.SerialException, PermissionError, OSError) as e:
            print(f"[重试] 无法打开 {device}: {e}", file=sys.stderr)
            time.sleep(2)

    try:
        while True:
            waiting = ser.in_waiting
            if waiting:
                raw = ser.read(waiting)
                text = raw.decode("utf-8", errors="replace").strip()
                if text:
                    timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                    # 高亮匹配的关键词
                    highlight = ""
                    for kw in HIGHLIGHT_KEYWORDS:
                        if kw in text.upper():
                            highlight = f"  ★★★ 命中关键词: {kw} ★★★"
                            break
                    print(f"[{timestamp}] {text}{highlight}")
            else:
                time.sleep(0.05)  # 空闲时休眠，降低 CPU 占用
    except KeyboardInterrupt:
        print("\n[退出] 用户中断，关闭串口。")
    finally:
        try:
            ser.close()
        except Exception:
            pass


if __name__ == "__main__":
    main()
