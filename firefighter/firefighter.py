from subsystems import Drivetrain

drivetrain = Drivetrain()

while (True):
    # testSubsystem.oscillate_motor()
    drivetrain.move_forward()
