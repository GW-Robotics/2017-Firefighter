import robotmap

from nanpy import ArduinoApi, SerialManager
from sensors import FlameSensor
from time import sleep

try:
    connection = SerialManager(device = '/dev/ttyACM0')
    Arduino = ArduinoApi(connection = connection)
except:
    pass

Arduino.pinMode(13, Arduino.OUTPUT)

flamesensor = FlameSensor(robotmap.get_pin('input', 'flamesensor'))

while (True):
    print flamesensor.is_flame_detected()
    Arduino.digitalWrite(13, Arduino.HIGH)
    sleep(0.3)
    Arduino.digitalWrite(13, Arduino.LOW)
    sleep(0.3)
