from subsystems import Drivetrain, StatusLight, Extinguisher
from gwrobolib.serialgamepad import F310
from time import sleep

#import multiprocessing

#def worker():
#    print "Robot in running"
#    return

#p = multiprocessing.Process(target=worker)
#p.start()
#p.join()

drivetrain = Drivetrain()
extinguisher = Extinguisher()

gamepad = F310()

robot_on = False
found_flame = False
    
# Equivalent to Arduino loop() method
while (not (status_light.is_freq_start() or status_light.activation_switch_pressed())):
    drivetrain.ultrasonic_check()
    
while (not extinguisher.has_flame()):
    drivetrain.naviguess_maze(0.3)
    
drivetrain.stop()
extinguisher.on()

while (True):
    drivetrain.pulse_to_distance(0.3, 3)
    pass
