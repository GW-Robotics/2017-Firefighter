from subsystems import Drivetrain

# H-drive drivetrain
drivetrain = Drivetrain()

while (True):
    drivetrain.arcade_drive(0.0, 0.0, 0.0)
