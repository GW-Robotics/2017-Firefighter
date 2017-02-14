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


class Drivetrain(object):

    def __init__(self):
        self.left_motor1 = Motor(12, 13)
        self.left_motor2 = Motor(6, 7)
        self.right_motor1 = Motor(2, 3)
        self.right_motor2 = Motor(4, 5)
        self.strafe_motor = Motor(8, 9)

    def move_forward(self):
        # Sets left and right motors to forward
        self.left_motor1.set(1.0)
        self.left_motor2.set(1.0)
        self.right_motor1.set(-1.0)
        self.right_motor1.set(-1.0)
        self.strafe_motor.set(0.0)    # Here in case stafe wheel was previously moving

    def arcade_drive(self, move_value, rotate_value):
        # Moves robot with a forward/backwards and rotation value
        
        move_value = robotmath.make_within(move_value, -1.0, 1.0)
        rotate_value = robotmath.make_within(rotate_value, -1.0, 1.0)

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

        set_left_right_outputs(left_motor_speed, right_motor_speed)

    def set_left_right_outputs(left_speed, right_speed):
        self.left_motor1.set(left_speed)
        self.left_motor2.set(left_speed)
        self.right_motor1.set(-right_speed)
        self.right_motor1.set(-right_speed)
        self.strafe_motor.set(0.0)    # Here in case stafe wheel was previously moving
