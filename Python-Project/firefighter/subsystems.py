from gwrobolib import robotmap, robotmath
from gwrobolib.sensors import FlameSensor, LimitSwitch
from gwrobolib.actuators import Motor
from gwrobolib.robotdrive import RobotDrive

from nanpy import ArduinoApi, SerialManager, Servo, Ultrasonic
from nanpy.counter import Counter
from time import sleep


# Connects the arduino and the raspberry pi through serial
try:
#    connection = SerialManager(device = '/dev/ttyACM0')
    connection = SerialManager()
    Arduino = ArduinoApi(connection = connection)
except:
    pass


class StatusLight(object):
    LOW_START = 3230
    HIGH_START = 4370
    
    def __init__(self):
        #self.LED = robotmap.get_pin('o', 'flame-led')

        self.activation_switch = LimitSwitch(robotmap.get_pin('i', 'activation-switch'))
        self.counter = Counter()
        #Arduino.pinMode(self.LED, Arduino.OUTPUT)

    def activation_switch_pressed(self):
        return self.activation_switch.get()
        
    def check_flame(self):
        #if self.flame_sensor.is_flame_detected():
        #    Arduino.digitalWrite(self.LED, Arduino.HIGH)
        #else:
        #    Arduino.digitalWrite(self.LED, Arduino.LOW)
        pass

    def is_freq_start(self):
        return self.counter.read_frequency() in range(self.LOW_START, self.HIGH_START)
        

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


class Extinguisher(object):

    def __init__(self):
        self.extinguisher = Motor(robotmap.get_pin('o', 'extinguisher-a'), robotmap.get_pin('o', 'extinguisher-b'))
        self.flame_sensor = FlameSensor(robotmap.get_pin('i', 'flame-sensor'))

    def has_flame(self):
        return self.flame_sensor.is_flame_detected()

    def on(self):
        self.extinguisher.set(1.0)

    def off(self):
        self.extinguisher.set(0.0)

class Drivetrain(object):

    target_forward_wall_distance = 5    # When ultrasonic is triggered by the wall (in inches)
    target_side_wall_distance = 3

    def __init__(self):
        self.drivetrain = RobotDrive(Motor(robotmap.get_pin('o', 'left-motor1a'), robotmap.get_pin('o', 'left-motor1b'))
                                     , Motor(robotmap.get_pin('o', 'left-motor2a'), robotmap.get_pin('o', 'left-motor2b'))
                                     , Motor(robotmap.get_pin('o', 'right-motor1a'), robotmap.get_pin('o', 'right-motor1b'))
                                     , Motor(robotmap.get_pin('o', 'right-motor2a'), robotmap.get_pin('o', 'right-motor2b'))
                                     , Motor(robotmap.get_pin('o', 'strafe-motora'), robotmap.get_pin('o', 'strafe-motorb')))

        self.front_ultrasonic = Ultrasonic(robotmap.get_pin('i', 'front-ultrasonic-e'), robotmap.get_pin('i', 'front-ultrasonic-t'), True)
        self.left_ultrasonic = Ultrasonic(robotmap.get_pin('i', 'left-ultrasonic-e'), robotmap.get_pin('i', 'left-ultrasonic-t'), True)
        self.right_ultrasonic = Ultrasonic(robotmap.get_pin('i', 'right-ultrasonic-e'), robotmap.get_pin('i', 'right-ultrasonic-t'), True)

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

    def naviguess_maze(speed):
        front_triggered = front_ultrasonic.get_distance() < 2
        left_triggered = left_ultrasonic.get_distance() < 2
        right_triggered = right_ultrasonic.get_distance() < 2
        right_favor = right_ultrasonic.get_distance() < 7 && right_ultrasonic.get_distance() > 2 && front_ultrasonic.get_distance()  <  20

        direction = True
        left_turns = 0
        right_turns = 0
    
        if front_triggered:
            arcade_drive( speed, 0 , 0)
            sleep( 0.1 )
            
            if left_triggered:
                if right_turns ==  4:
                    turn_to_angle(-90, speed) #implement later
                    right_turns = 0
                else:
                    turn_to_angle(90, -speed)
                    right_turns++
            else if right_triggered:
                if left_turns == 4:
                    turn_to_angle(-90, -speed)
                    left_turns = 0
                else:
                    turn_to_angle(-90, speed)
                    left_turns++
            else:
                if right_turns == 4:
                    turn_to_angle(-90, speed)
                    right_turns = 0
                else:
                    turn_to_angle(90, -speed)
                    right_turns++
        else:
            if left_triggered:
                turn_to_angle(1, -speed)
            else if right_favored:
                arcade_drive(0, 0, speed)
                while right_ultrasonic.get_distance() > 2.5:
                    pass
                arcade_drive(0, 0, 0)
            else:
                arcade_drive(speed, 0, 0)
                
            
        if direction:
            arcade_drive(0, 0.3, 0) #turn on strafe motor
        else:
            arcade_drive(0, -0.3, 0)

        direction = !direction

        sleep(0.05)
