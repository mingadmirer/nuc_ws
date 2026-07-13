#!/usr/bin/env python3
"""
等待 STM32 上电并就绪。
反复发送 Mstop00000，直到收到 "Car stopped" 才退出。
"""

import serial
import time
import sys

DEVICE = "/dev/serial/by-id/usb-STMicroelectronics_STM32_Virtual_ComPort_326B326C3034-if00"
BAUDRATE = 115200

retry = 0
while True:
    retry += 1
    try:
        ser = serial.Serial(DEVICE, BAUDRATE, timeout=1)
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        ser.write(b"Mstop00000")
        time.sleep(0.3)

        waiting = ser.in_waiting
        if waiting:
            resp = ser.read(waiting)
            text = resp.decode("utf-8", errors="replace").strip()
            print(f"[第{retry:02d}次] 返回: {text}")
            if "Car stopped" in text:
                print(f"✓ STM32 固件就绪！（{retry}次握手成功）")
                ser.close()
                sys.exit(0)
        else:
            print(f"[第{retry:02d}次] 无应答，重试...")

        ser.close()
    except (serial.SerialException, OSError) as e:
        print(f"[第{retry:02d}次] 串口未就绪: {e}")

    time.sleep(1)
