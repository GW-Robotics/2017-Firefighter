from subsystems import Drivetrain, StatusLight, Extinguisher
#from subsystems import SensorStick    # SEAS Innovation Challenge subsystems
from time import sleep

#import multiprocessing

#def worker():
#    print "Robot in running"
#    return

#p = multiprocessing.Process(target=worker)
#p.start()
#p.join()

# H-drive drivetrain
drivetrain = Drivetrain()
# sensor_stick = SensorStick()
status_light = StatusLight()
extinguisher = Extinguisher()

robot_on = False
found_flame = False
    
# Equivalent to Arduino loop() method
while (not (status_light.is_freq_start() or status_light.activation_switch_pressed())):
    pass
    
while (not extinguisher.has_flame()):
    drivetrain.naviguess_maze(0.3)
    
drivetrain.stop()
extinguisher.on()

while (True):
    drivetrain.pulse_to_distance(0.3, 3)
    pass
