from nanpy import ArduinoApi, SerialManager
from time import sleep

# Connects the arduino and the raspberry pi through serial
try:
    connection = SerialManager()
    Arduino = ArduinoApi(connection = connection)
except:
    pass

class FlameSensor(object):
# Sensor that detects flames

    def __init__(self, pin):
        self.pin = pin
        print 'Flame Sensor Connected To: ' + self.pin

        Arduino.pinMode(self.pin, Arduino.INPUT)

    def is_flame_detected(self):
        return Arduino.digitalRead(self.pin) == Arduino.LOW

class Sonar(object):
# Sensor that finds distance from an object

    def __init__(self, trig, echo, useInches):
        self.trig = trig
        self.echo = echo
        self.useInches = useInches

        if useInches:
            self.dist_conversion = 0.013504
        else:
            self.dist_conversion = 0.034
        
        Arduino.pinMode(self.trig, Arduino.OUTPUT)
        Arduino.pinMode(self.echo, Arduino.INPUT)

    def read_sensor(self):
        #Used by get_cm and get_in, do not call this function
        #Send low trigger
        Arduino.digitalWrite(self.trig, Arduino.LOW)
        sleep(0.0000002)    # 2 microsecond delay

        #Send high trigger
        Arduino.digitalWrite(self.trig, Arduino.HIGH)
        sleep(0.000001)     # 10 microsecond delay

        #Send another low trigger
        Arduino.digitalWrite(self.trig,Arduino.LOW)

        #receive the signal
        return Arduino.pulseIn(self.echo, Arduino.HIGH)
    
    def get_distance(self):
        duration = self.read_sensor()
        return duration / 2. * self.dist_conversion

    def reading_in_range(self, low, high):
        reading = self.get_distance()
        return reading > low and reading < high

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
