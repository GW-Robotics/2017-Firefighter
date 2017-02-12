from subsystems import TestSubsystem, Drivetrain


testSubsystem = TestSubsystem()
drivetrain = Drivetrain()

while (True):
    # testSubsystem.oscillate_motor()
    drivetrain.move_forward()
