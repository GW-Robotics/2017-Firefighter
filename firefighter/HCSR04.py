from nanpy import ArduinoApi, SerialManager



class Sonar(object):
    a = ArduinoApi()
    def __init__(self, trig, echo):
        self.trig = trig
        self.echo = echo
        a.pinMode(trig, a.OUTPUT)
        a.pinMode(echo, a.INPUT)
    def raed_sensor(self):
        #Used by get_cm and get_in, do not call this function
        #Send low trigger
        a.digitalWrite(trig, a.LOW)
        a.delayMicroseconds(2)

        #Send high trigger
        a.digitalWrite(trig, a.HIGH)
        a.delayMicroseconds(10);

        #Send another low trigger
        a.digitalWrite(trig,a.LOW)

        #recieve the signal
        return a.pulseIn(echo, a.HIGH)

        return duration
    
    def get_cm(self):
        duration = self.read_sensor()
        return duration / 2. * .034
    def get_in(self):
        duration = self.read_sensor()
        return duration / 2. * .013504
    def reading_in_range_cm(self, low, high):
        reading = self.get_cm()
        return reading > low and reading < high
    def reading_in_range_in(self, low, high):
        reading = self.get_in()
        return reading > low and reading < high




