from nanpy import ArduinoApi, SerialManager

try:
    connection = SerialManager(device = '/dev/ttyACM0')
    Arduino = ArduinoApi(connection = connection)
except:
    pass

class FlameSensor(object):
    
    def __init__(self, pin):
        self.sensor_pin = pin
        print 'Flame Sensor Connected To: ' + self.sensor_pin

        Arduino.pinMode(self.sensor_pin, Arduino.INPUT)

    def is_flame_detected(self):
        return Arduino.digitalRead(self.sensor_pin) == Arduino.HIGH
