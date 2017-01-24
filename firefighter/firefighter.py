import os
from ConfigParser import ConfigParser
from nanpy import ArduinoApi, SerialManager

assert os.path.exists('firefighter/wiring.cfg')

connected_to_arduino = False
VERBOSE = True

try:
    connection = SerialManager(device = '/dev/ttyACM0')
    Arduino = ArduinoApi(connection = connection)
    connected_to_arduino = True
except:
    connected_to_arduino = False


# Motor class that will be used to control individual motors on the robot
class Motor(object):
    
    def __init__(self, pin):
        self.pin = pin
        if connected_to_arduino:
            Arduino.pinMode(self.pin, Arduino.OUTPUT)
        
    def set_speed(self, speed):
        if VERBOSE:
            print "Setting speed to " + str(speed)
            
        if connected_to_arduino:
            Arduino.digitalWrite(speed)


# Reads the config file into the program
config = ConfigParser()
config.read('firefighter/wiring.cfg')

input_pins = config.items("Input")
output_pins = config.items("Output")

# Gets the value of a pin based on the name of a value in the wiring.cfg file
def get_pin(pin_type, name):
    if pin_type == 'input':
        for pair in input_pins:
            if pair[0] == name:
                return pair[1]
    elif pin_type == 'output':
        for pair in output_pins:
            if pair[0] == name:
                return pair[1]

# # Example of making a motor and setting its speed to 100%
# input_example1 = Motor(get_pin('input', 'input_example1'))
# input_example1.set_speed(1.0)