import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200)

ser.write(b'Mfwrd04000')
