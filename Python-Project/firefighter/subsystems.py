from gwrobolib import robotmap, robotmath
from gwrobolib.sensors import FlameSensor, LimitSwitch
from gwrobolib.actuators import Motor
from gwrobolib.robotdrive import RobotDrive

from nanpy import ArduinoApi, SerialManager, Servo, Ultrasonic
from time import sleep


# Connects the arduino and the raspberry pi through serial
try:
#    connection = SerialManager(device = '/dev/ttyACM0')
    connection = SerialManager()
    Arduino = ArduinoApi(connection = connection)
except:
    pass


class StatusLight(object):

    def __init__(self):
        #self.LED = robotmap.get_pin('o', 'flame-led')
        #self.flame_sensor = FlameSensor(robotmap.get_pin('i', 'flame-sensor'))

        self.activation_switch = LimitSwitch(robotmap.get_pin('i', 'activation-switch'))
        #Arduino.pinMode(self.LED, Arduino.OUTPUT)

    def activation_switch_pressed(self):
        return self.activation_switch.get()
        
    def check_flame(self):
        #if self.flame_sensor.is_flame_detected():
        #    Arduino.digitalWrite(self.LED, Arduino.HIGH)
        #else:
        #    Arduino.digitalWrite(self.LED, Arduino.LOW)
        pass
        

class SensorStick(object):
    
    def __init__(self):
        self.servo = Servo(robotmap.get_pin('o', 'sensor-stick-servo'), connection)
        self.switch = LimitSwitch(robotmap.get_pin('i', 'sensor-stick-switch'))
        self.out = (self.servo.read() > 100)

    def switch_state(self):
        return self.switch.get()

    def servo_position(self):
        return self.servo.read()

    def check_actuation(self):
        if self.switch.get():
            if self.out == True:
                self.servo.write(90)
                self.out = False
            else:
                self.servo.write(180)
                self.out = True
        else:
            pass
        

class Drivetrain(object):

    target_forward_wall_distance = 5    # When ultrasonic is triggered by the wall (in inches)
    target_side_wall_distance = 3

    def __init__(self):
        self.drivetrain = RobotDrive(Motor(robotmap.get_pin('o', 'left-motor1a'), robotmap.get_pin('o', 'left-motor1b'), robotmap.get_pin('o', 'left-motor1p'))
                                     , Motor(robotmap.get_pin('o', 'left-motor2a'), robotmap.get_pin('o', 'left-motor2b'), robotmap.get_pin('o', 'left-motor2p'))
                                     , Motor(robotmap.get_pin('o', 'right-motor1a'), robotmap.get_pin('o', 'right-motor1b'), robotmap.get_pin('o', 'right-motor1p'))
                                     , Motor(robotmap.get_pin('o', 'right-motor2a'), robotmap.get_pin('o', 'right-motor2b'), robotmap.get_pin('o', 'right-motor2p'))
                                     , Motor(robotmap.get_pin('o', 'strafe-motora'), robotmap.get_pin('o', 'strafe-motorb'), robotmap.get_pin('o', 'strafe-motorp')))

        self.front_ultrasonic = Ultrasonic(robotmap.get_pin('i', 'front-ultrasonic-e'), robotmap.get_pin('i', 'front-ultrasonic-t'), True)
        self.left_ultrasonic = Ultrasonic(robotmap.get_pin('i', 'left-ultrasonic-e'), robotmap.get_pin('i', 'left-ultrasonic-t'), True)
        self.right_ultrasonic = Ultrasonic(robotmap.get_pin('i', 'right-ultrasonic-e'), robotmap.get_pin('i', 'right-ultrasonic-t'), True)
        self.back_ultrasonic = Ultrasonic(robotmap.get_pin('i', 'back-ultrasonic-e'), robotmap.get_pin('i', 'back-ultrasonic-t'), True)

    def arcade_drive(self, move_value, rotate_value, strafe_value):
        self.drivetrain.h_drive(move_value, rotate_value, strafe_value)

    def stop(self):
        self.drivetrain.stop()

    def all_test(self, value):
        self.drivetrain.all_test(value)

    def drive_to_front(self):
        self.arcade_drive(0.3, 0.0, 0.0)
        
        while (not self.front_ultrasonic.reading_in_range(0, self.target_forward_wall_distance)):
            pass

        self.stop()
    
    def navigate_maze(self):
        """Simple Left-Handed Wall Follower Algorithm"""
        arcade_drive(1.0, 0.0, 0.0)
        
        if self.front_ultrasonic.reading_in_range(0, self.target_forward_wall_distance):
            arcade_drive(0.0, 0.0, -1.0)

        if self.left_ultrasonic.reading_in_range(0, self.target_side_wall_distance):
            arcade_drive(0.0, 0.0, 1.0)

        if self.right_ultrasonic.reading_in_range(0, self.target_forwardwall_distance):
            arcade_drive(0.0, 1.0, 0.0)
            sleep(1.0)
            arcade_drive(1.0, 0.0, 0.0)

