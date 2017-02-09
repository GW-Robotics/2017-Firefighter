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

flamesensor = FlameSensor(robotmap.get_pin('input', 'flamesensor'))
ultrasonic = Sonar(50, 52, True)
colorSensor = ColorSensor(24, 26, 28, 30, 32)

while (True):
    print flamesensor.is_flame_detected()
#   print ultrasonic.read_sensor()
    print colorSensor.get_color('b')
    sleep(0.6)
