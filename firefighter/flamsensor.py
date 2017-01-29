from nanpy import ArduinoApi, SerialManager

Arduino = ArduinoApi()

class FlameSensor(Object):

    def __init__(self, pin):
        self.sensor_pin = pin
        Arduino.pinMode(sensor_pin, Arduino.INPUT)

    def is_flame_detected(self):
        return Arduino.digitalRead(self.sensor_pin) == Arduino.HIGH
