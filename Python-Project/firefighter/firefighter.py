from subsystems import Drivetrain, StatusLight
#from subsystems import SensorStick    # SEAS Innovation Challenge subsystems
from time import sleep

import multiprocessing

def worker():
    print "Robot in running"
    return

p = multiprocessing.Process(target=worker)
p.start()
p.join()

# H-drive drivetrain
drivetrain = Drivetrain()
# sensor_stick = SensorStick()
status_light = StatusLight()

#testsystem = Test()

def drive_in_square():
    drivetrain.arcade_drive(1.0, 0.0, 0.0)
    sleep(1)
    drivetrain.arcade_drive(0.0, 0.0, 1.0)
    sleep(1)
    drivetrain.arcade_drive(-1.0, 0.0, 0.0)
    sleep(1)
    drivetrain.arcade_drive(0.0, 0.0, -1.0)
    sleep(1)

while (not (status_light.is_freq_start() or status_light.activation_switch_pressed())):
    pass

sleep(2)
print "Started!"

while (not status_light.activation_switch_pressed()):
    #sensor_stick.check_actuation()
    #drivetrain.drive_to_front()
    #drive_in_square()
    pass
    
drivetrain.stop()
