import robotmap

from nanpy import ArduinoApi, SerialManager
from sensors import FlameSensor

connected_to_arduino = False

try:
    connection = SerialManager(device = '/dev/ttyACM0')
    Arduino = ArduinoApi(connection = connection)
    connected_to_arduino = True
except:
    connected_to_arduino = False
    

class FlameSensor(object):

    def __init__(self, pin):
        self.sensor_pin = pin
        print 'Flame Sensor Connected To: ' + self.sensor_pin

        Arduino.pinMode(self.sensor_pin, Arduino.INPUT)

    def is_flame_detected(self):
        return Arduino.digitalRead(self.sensor_pin) == Arduino.HIGH


flamesensor = FlameSensor(robotmap.get_pin('input', 'flamesensor'))

print flamesensor.is_flame_detected()
