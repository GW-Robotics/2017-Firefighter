import robotmath

from nanpy import ArduinoApi, SerialManager
from time import sleep

# Connects the arduino and the raspberry pi through serial
try:
#    connection = SerialManager(device = '/dev/ttyACM0')
    connection = SerialManager()
    Arduino = ArduinoApi(connection = connection)
except:
    pass


class Motor(object):

    def __init__(self, pinA, pinB, pwm = -1):
        '''Motor controlled by Dual H-Bridge Motor Driver'''
        self.pinA = pinA
        self.pinB = pinB
        self.pwm = pwm

        Arduino.pinMode(self.pinA, Arduino.OUTPUT)
        Arduino.pinMode(self.pinB, Arduino.OUTPUT)

        if not self.pwm == -1:
            Arduino.pinMode(self.pwm, Arduino.OUTPUT)

        print "Motor created at [{}, {}, {}]...".format(self.pinA, self.pinB, self.pwm)

    def set(self, speed):
        if not self.pwm == -1:
            Arduino.analogWrite(self.pwm, 255 * abs(speed))
            
            if speed > 0.0:
                Arduino.digitalWrite(self.pinA, Arduino.HIGH)
                Arduino.digitalWrite(self.pinB, Arduino.LOW)
            elif speed < 0.0:
                Arduino.digitalWrite(self.pinA, Arduino.LOW)
                Arduino.digitalWrite(self.pinB, Arduino.HIGH)
            else:
                Arduino.digitalWrite(self.pinA, Arduino.LOW)
                Arduino.digitalWrite(self.pinB, Arduino.LOW)
        else:
            if speed > 0.0:
                Arduino.analogWrite(self.pinA, abs(speed) * 255)
                Arduino.analogWrite(self.pinB, 0.0)
            elif speed < 0.0:
                Arduino.analogWrite(self.pinA, 0.0)
                Arduino.analogWrite(self.pinB, abs(speed) * 255)
            else:
                Arduino.analogWrite(self.pinA, 0.0)
                Arduino.analogWrite(self.pinB, 0.0)

class Solenoid(object):

    def __init__(self, pinA, pinB):
        '''Solenoid controlled by Dual H-Bridge Motor Driver'''
        self.pinA = pinA
        self.pinB = pinB

    def on():
        Arduino.digitalWrite(self.pinA, Arduino.HIGH)
        Arduino.digitalWrite(self.pinB, Arduino.LOW)

    def off():
        Arduino.digitalWrite(self.pinA, Arduino.LOW)
        Arduino.digitalWrite(self.pinB, Arduino.LOW)


