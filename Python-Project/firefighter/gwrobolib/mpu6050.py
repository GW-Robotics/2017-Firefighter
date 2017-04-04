from nanpy import ArduinoApi, SerialManager, wire
from nanpy.i2c import I2C_Master
import robotmath
import math

# Connects the arduino and the raspberry pi through serial
try:
    connection = SerialManager(device = 'COM4')
#    connection = SerialManager()
    Arduino = ArduinoApi(connection = self.connection)
except:
    print 'Error: No Arduino found...'


class MPU6050(object):
    MPU_address = 0x68
    minVal = 265
    maxVal = 402
    
    def __init__(self):
        self.master = I2C_Master(wire.Wire(connection = connection))
        self.master.send(self.MPU_address, [0x6B, 0])

    def getAngle(self, axis):
        data = self.master.request(self.MPU_address, 14)

        accX = data[0] << 8 | data[1]
        accY = data[2] << 8 | data[3]
        accZ = data[4] << 8 | data[5]

        xAngle = robotmath.map(accX, minVal, maxVal, -90, 90)
        yAngle = robotmath.map(accY, minVal, maxVal, -90, 90)
        zAngle = robotmath.map(accZ, minVal, maxVal, -90, 90)
        
        if (axis == 'x'):
            return math.toDegrees(atan2(-yAngle, -zAngle) + math.pi) 
        elif (axis == 'y'):
            return math.toDegrees(atan2(-xAngle, -zAngle) + math.pi)
        elif (axist == 'z'):
            return math.toDegrees(atan2(-yAngle, -xAngle) + math.pi)

gyro = MPU6050()
        
while (True):
    self.gyro.getAngle('z')
