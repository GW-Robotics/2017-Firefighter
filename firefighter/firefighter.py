from ConfigParser import ConfigParser

def get_pin(pin_type, name):
    if pin_type == 'input':
        for pair in input_pins:
            if pair[0] == name:
                return pair[1]
    elif pin_type == 'output':
        for pair in output_pins:
            if pair[0] == name:
                return pair[1]

config = ConfigParser()
config.read('wiring.cfg')

input_pins = config.items("Input")
output_pins = config.items("Output")

print get_pin('input', 'input_example1')
print get_pin('output', 'output_example1')


