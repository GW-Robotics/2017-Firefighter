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
mpu = mpu6050(0x68)#address param taken from example 

robot_on = False
found_flame = False
    
# Equivalent to Arduino loop() method
while (not (status_light.is_freq_start() or status_light.activation_switch_pressed())):
    pass
    
while (not extinguisher.has_flame()):
    drivetrain.naviguess_maze(0.3)
    
drivetrain.stop()
extinguisher.on()

def turn_to_angle(target, speed):
    arcade_drive(0, speed, 0)

    while( abs(get_angle) < target_angle):
        pass
    arcade_drive(0, 0, 0)

def get_angle( axis ):
    gyro_value = mpu.get_gyro_data()
    #return format [x,y,z]
    return {
        'x': gyro_value[0],
        'y': gyro_value[1],
        'z': gyro_value[2],
    }.get(axis, None)

while (True):
    drivetrain.pulse_to_distance(0.3, 3)
    pass
