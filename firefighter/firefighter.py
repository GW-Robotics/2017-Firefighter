from subsystems import Drivetrain

# H-drive drivetrain
drivetrain = Drivetrain()

while (True):
    # testSubsystem.oscillate_motor()
    drivetrain.move_forward()
