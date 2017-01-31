from nanpy import ArduinoApi, SerialManager
from time import sleep

# Connects the arduino and the raspberry pi through serial
try:
    connection = SerialManager(device = '/dev/ttyACM0')
    Arduino = ArduinoApi(connection = connection)
except:
    pass

class FlameSensor(object):
# Sensor that detects flames

    def __init__(self, pin):
        self.pin = pin
        print 'Flame Sensor Connected To: ' + self.pin

        Arduino.pinMode(self.sensor_pin, Arduino.INPUT)

    def is_flame_detected(self):
        return Arduino.digitalRead(self.pin) == Arduino.HIGH

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
        
        Arduino.pinMode(trig, Arduino.OUTPUT)
        Arduino.pinMode(echo, Arduino.INPUT)

    def read_sensor(self):
        #Used by get_cm and get_in, do not call this function
        #Send low trigger
        Arduino.digitalWrite(trig, Arduino.LOW)
        sleep(0.0000002)    # 2 microsecond delay

        #Send high trigger
        Arduino.digitalWrite(trig, Arduino.HIGH)
        sleep(0.000001)     # 10 microsecond delay

        #Send another low trigger
        Arduino.digitalWrite(trig,Arduino.LOW)

        #receive the signal
        return Arduino.pulseIn(echo, Arduino.HIGH)
    
    def get_distance(self):
        duration = self.read_sensor()
        return duration / 2. * self.dist_conversion

    def reading_in_range_cm(self, low, high):
        reading = self.get_distance()
        return reading > low and reading < high
