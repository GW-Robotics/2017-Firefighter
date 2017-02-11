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


class TestSubsystem(object):

    flamesensor = FlameSensor(robotmap.get_pin('input', 'flamesensor'))
    ultrasonic = Sonar(50, 52, True)
    colorSensor = ColorSensor(24, 26, 28, 30, 32)

    
    def __init__(self):
        self.test_motor = Motor(8, 9)


    def oscillate_motor(self):
        self.test_motor.set(-1.0)
        sleep(1)
        self.test_motor.set(1.0)
        sleep(1)


    def sensor_test(self):
        print flamesensor.is_flame_detected()
        print ultrasonic.read_sensor()
        print colorSensor.get_color('b')

