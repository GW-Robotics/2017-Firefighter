def naviguess_maze( speed ):
    front_triggered = front_ultrasonic.get_distance() < 2
    left_triggered = left_ultrasonic.get_distance() < 2
    right_triggered = right_ultrasonic.get_distance() < 2
    right_favor = right_ultrasonic.get_distance() < 7 && right_ultrasonic.get_distance() > 2 && front_ultrasonic.get_distance()  <  20

    direction = True
    left_turns = 0
    right_turns = 0
    
    if front_triggered:
        arcade_drive( speed, 0 , 0)
        sleep( 0.1 )

        if left_triggered:
            if right_turns ==  4:
                turn_to_angle(-90, speed) #implement later
                right_turns = 0
            else:
                turn_to_angle(90, -speed)
                right_turns++
            else if right_triggered:
                if left_turns == 4:
                    turn_to_angle(-90, -speed)
                    left_turns = 0
                else:
                    turn_to_angle(-90, speed)
                    left_turns++
            else:
                if right_turns == 4:
                    turn_to_angle(-90, speed)
                    right_turns = 0
                else:
                    turn_to_angle(90, -speed)
                    right_turns++
        else:
            if left_triggered:
                turn_to_angle(1, -speed)
            else if right_favored:
                arcade_drive(0, 0, speed)
                while right_ultrasonic.get_distance() > 2.5:
                    pass
                arcade_drive(0, 0, 0)
            else:
                arcade_drive(speed, 0, 0)
                
            
        if direction:
            arcade_drive(0, 0.3, 0) #turn on strafe motor
        else:
            arcade_drive(0, -0.3, 0)

        direction = !direction

        sleep(0.05)
    
