from subsystems import Drivetrain, StatusLight
from time import sleep
import sensors

# H-drive drivetrain
drivetrain = Drivetrain()
status_light = StatusLight()

while (True):
    status_light.check_flame()
    drivetrain.arcade_drive(1.0, 0.0, 0.0)
    sleep(.1)
