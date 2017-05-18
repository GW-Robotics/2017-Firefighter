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

        self.left_motor1.set(robotmath.make_within(left_motor_speed, -1.0, 1.0))
        self.left_motor2.set(robotmath.make_within(left_motor_speed, -1.0, 1.0))
        self.right_motor1.set(-robotmath.make_within(right_motor_speed, -1.0, 1.0))
        self.right_motor2.set(-robotmath.make_within(right_motor_speed, -1.0, 1.0))
        self.strafe_motor.set(strafe_value)

    
    def stop(self):
        self.left_motor1.set(0.0)
        self.left_motor2.set(0.0)
        self.right_motor1.set(0.0)
        self.right_motor2.set(0.0)
        self.strafe_motor.set(0.0)


    def all_test(self, value):
        self.left_motor1.set(value)
        self.left_motor2.set(value)
        self.right_motor1.set(value)
        self.right_motor2.set(value)
        self.strafe_motor.set(value)
