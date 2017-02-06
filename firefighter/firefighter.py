import robotmap

from nanpy import ArduinoApi, SerialManager
from sensors import FlameSensor
from time import sleep

# Connects the arduino and the raspberry pi through serial
try:
    connection = SerialManager(device = '/dev/ttyACM0')
    Arduino = ArduinoApi(connection = connection)
except:
    pass

Arduino.pinMode(13, Arduino.OUTPUT)

flamesensor = FlameSensor(robotmap.get_pin('input', 'flamesensor'))

raw_input("Press ENTER to enable robot")

while (True):
    print flamesensor.is_flame_detected()
    Arduino.digitalWrite(13, Arduino.HIGH)
    sleep(0.3)
    Arduino.digitalWrite(13, Arduino.LOW)
    sleep(0.3)

def detected_sound(self):
    # Runs when a sound is detected
    pass

def detected_flame(self):
    # Runs when a flame is detected
    pass

def detected_video(self):
    # Runs when a computer vision is being used:
    # Solid: baby drop-off target detected
    # Blinking: cradle target detected
    pass


