import robotmap

from nanpy import ArduinoApi, SerialManager
from sensors import FlameSensor, Sonar, ColorSensor
from time import sleep


# Connects the arduino and the raspberry pi through serial
try:
    connection = SerialManager()
    Arduino = ArduinoApi(connection = connection)
except:
    pass


class Motor(object):

    def __init__(self, pinA, pinB, pwm = -1):
        self.pinA = pinA
        self.pinB = pinB
        self.pwm = pwm

        Arduino.pinMode(self.pinA, Arduino.OUTPUT)
        Arduino.pinMode(self.pinB, Arduino.OUTPUT)

        if not pwm == -1:
            Arduino.pinMode(self.pwm, Arduino.OUTPUT)

    def set(self, speed):
        if speed > 0.0:
            Arduino.digitalWrite(self.pinA, Arduino.HIGH)
            Arduino.digitalWrite(self.pinB, Arduino.LOW)
        elif speed < 0.0:
            Arduino.digitalWrite(self.pinA, Arduino.LOW)
            Arduino.digitalWrite(self.pinB, Arduino.HIGH)
        else:
            Arduino.digitalWrite(self.pinA, Arduino.LOW)
            Arduino.digitalWrite(self.pinB, Arduino.LOW)


class Drivetrain(object):

    def __init__(self):
        self.left_motor1 = Motor(12, 13)
        self.left_motor2 = Motor(6, 7)
        self.right_motor1 = Motor(2, 3)
        self.right_motor2 = Motor(4, 5)
        self.strafe_motor = Motor(8, 9)

    def move_forward(self):
        self.left_motor1.set(1.0)
        self.left_motor2.set(1.0)
        self.right_motor1.set(-1.0)
        self.right_motor1.set(-1.0)
        self.strafe_motor.set(0.0)    # Here in case stafe wheel was previously moving
