from ast import literal_eval
from time import sleep

import serial

class F310(object):

    def __init__(self):
        self.ser = serial.Serial("/dev/rfcomm0", baudrate = 9600, timeout = 0.5)

    def get_values(self):
        self.current_line = literal_eval(self.ser.readline())
        return self.current_line

    def get_gamepad_input(self, value):
        return self.current_line[value]

if __name__ == "__main__":
    gamepad = F310()
    
    while True:
        gamepad.get_values()
        print "FB: {}, LR: {}, Strafe: {}, Shoot: {}".format(gamepad.get_gamepad_input('fb'), gamepad.get_gamepad_input('lr'), gamepad.get_gamepad_input('s'), gamepad.get_gamepad_input('c'))
        sleep(0.1)
