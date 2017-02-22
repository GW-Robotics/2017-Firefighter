import robotmap
import robotmath

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

class StatusLight(object):

    def __init__(self):
        self.LED = 13
        self.flame_sensor = FlameSensor(12)

        Arduino.pinMode(self.LED, Arduino.OUTPUT)

    def check_flame(self):
        if self.flame_sensor.is_flame_detected():
            Arduino.digitalWrite(self.LED, Arduino.HIGH)
        else:
            Arduino.digitalWrite(self.LED, Arduino.LOW)

class Drivetrain(object):

    def __init__(self):
        self.left_motor1 = Motor(robotmap.get_pin('o', 'left-motor1a'), robotmap.get_pin('o', 'left-motor1b'))
        self.left_motor2 = Motor(robotmap.get_pin('o', 'left-motor2a'), robotmap.get_pin('o', 'left-motor2b'))
        self.right_motor1 = Motor(robotmap.get_pin('o', 'right-motor1a'), robotmap.get_pin('o', 'right-motor1b'))
        self.right_motor2 = Motor(robotmap.get_pin('o', 'right-motor2a'), robotmap.get_pin('o', 'right-motor2b'))
        self.strafe_motor = Motor(robotmap.get_pin('o', 'strafe-motor1a'), robotmap.get_pin('o', 'strafe-motor1a'))

    def arcade_drive(self, move_value, rotate_value, strafe_value):
        # Moves robot with a forward/backwards and rotation value
        
        move_value = robotmath.make_within(move_value, -1.0, 1.0)
        rotate_value = robotmath.make_within(rotate_value, -1.0, 1.0)
        strafe_value = robotmath.make_within(strafe_value, -1.0, 1.0)
        
        if move_value > 0.0:
            if rotate_value > 0.0:
                left_motor_speed = move_value - rotate_value
                right_motor_speed = max(move_value, rotate_value)
            else:
                left_motor_speed = max(move_value, -rotate_value)
                right_motor_speed = move_value + rotate_value
        else:
            if rotate_value > 0.0:
                left_motor_speed = -max(-move_value, rotate_value)
                right_motor_speed = move_value + rotate_value
            else:
                left_motor_speed = move_value - rotate_value
                right_motor_speed = -max(-move_value, -rotate_value)

        self.left_motor1.set(left_motor_speed)
        self.left_motor2.set(left_motor_speed)
        self.right_motor1.set(-right_motor_speed)
        self.right_motor2.set(-right_motor_speed)
        self.strafe_motor.set(strafe_value)
