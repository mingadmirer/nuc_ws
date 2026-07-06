import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200)
#ser.write(b'Mbwrd01250')
#time.sleep(3)

# 夹爪复位
#ser.write(b'Mfwrs00090')
#ser.write(b'Mfing44444')
#ser.write(b'Mstop00000')

# 左转
#ser.write(b'Mltrn01000')
#time.sleep(1)

# 右转
#ser.write(b'Mrtrn01000')
#time.sleep(1)

# 左前进
#ser.write(b'Mltrl01250')
#time.sleep(2)

# 右前进
# ser.write(b'Mrtrl02000')
# time.sleep(3)
# ser.write(b'Mstop00000')
# 夹取、翻转、松开

ser.write(b'Mfing55555')
time.sleep(1)
ser.write(b'Mfing44444')
#time.sleep(0.5)
# ser.write(b'Mltrl02000')
# time.sleep(2)
# ser.write(b'Mstop00000')

# time.sleep(2)
#ser.write(b'Mfwrs00210')
#ser.write(b'Mstop00000')


# ser.write(b'Mupwd44444')
# time.sleep(2)