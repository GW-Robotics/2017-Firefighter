from nanpy import ArduinoApi, SerialManager
from time import sleep

# Connects the arduino and the raspberry pi through serial
try:
#    connection = SerialManager(device = '/dev/ttyACM0')
    connection = SerialManager()
    Arduino = ArduinoApi(connection = connection)
except:
    pass


class LimitSwitch(object):
# Sensor that detects touch

    def __init__(self, pin):
        self.pin = pin

        Arduino.pinMode(self.pin, Arduino.INPUT)

    def get(self):
	sleep(.01)
        return Arduino.digitalRead(self.pin) == Arduino.HIGH


class FlameSensor(object):
# Sensor that detects flames

    def __init__(self, pin):
        self.pin = pin

        Arduino.pinMode(self.pin, Arduino.INPUT)

    def is_flame_detected(self):
        return Arduino.digitalRead(self.pin) == Arduino.LOW

    
class ColorSensor(object):

    def __init__(self, s0, s1, s2, s3, sensorOut):
        self.s0 = s0
        self.s1 = s1
        self.s2 = s2
        self.s3 = s3
        self.sensorOut = sensorOut
        Arduino.pinMode(s0, Arduino.OUTPUT)
        Arduino.pinMode(s1, Arduino.OUTPUT)
        Arduino.pinMode(s2, Arduino.OUTPUT)
        Arduino.pinMode(s3, Arduino.OUTPUT)
        Arduino.pinMode(sensorOut, Arduino.INPUT)
        Arduino.digitalWrite(s0, Arduino.HIGH)
        Arduino.digitalWrite(s1, Arduino.LOW)

    def get_color(self, color):
        if color == 'r':
            #Return 0- ~255 red value
            Arduino.digitalWrite(self.s2, Arduino.HIGH)
            Arduino.digitalWrite(self.s3, Arduino.LOW)
        elif color == 'g':
            #return 0- ~255 green value
            Arduino.digitalWrite(self.s2, Arduino.HIGH)
            Arduino.digitalWrite(self.s3, Arduino.HIGH)
        elif color == 'b':
            #return 0- ~255 blue value 
            Arduino.digitalWrite(self.s2, Arduino.LOW)
            Arduino.digitalWrite(self.s3, Arduino.HIGH)
            
        return Arduino.pulseIn(self.sensorOut, Arduino.LOW)
