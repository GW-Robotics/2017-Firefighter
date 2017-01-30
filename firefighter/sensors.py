from nanpy import ArduinoApi, SerialManager

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
