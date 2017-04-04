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


class RobotDrive(object):

    def __init__(self, left_motor1, left_motor2, right_motor1, right_motor2, strafe_motor):
        self.left_motor1 = left_motor1
        self.left_motor2 = left_motor2
        self.right_motor1 = right_motor1
        self.right_motor2 = right_motor2
        self.strafe_motor = strafe_motor


    def h_drive(self, move_value, rotate_value, strafe_value):
        """Moves robot with a forward/backwards and rotation value"""
        
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

        
    def stop(self):
        self.left_motor1.set(0.0)
        self.left_motor2.set(0.0)
        self.right_motor1.set(0.0)
        self.right_motor2.set(0.0)
        self.strafe_motor.set(0.0)
