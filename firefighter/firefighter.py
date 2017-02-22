from subsystems import Drivetrain
from time import sleep
import sensors

# H-drive drivetrain
drivetrain = Drivetrain()
FlameSensor = sensors.FlameSensor(12)#Flame Sensor Using Pin 12
Flame_LED_Pin = 13#connect LED to pin 13
while (True):
    FlameSensor.is_flame_detected_LED(13)
    drivetrain.arcade_drive(1.0, 0.0, 0.0)
    time.sleep(.1)
