import os
from ConfigParser import ConfigParser

assert os.path.exists('wiring.cfg')

# Reads the config file into the program
config = ConfigParser()
config.read('wiring.cfg')

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
