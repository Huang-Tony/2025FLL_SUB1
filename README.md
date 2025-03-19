# CIS 2025 FLL INNOVATION REPO

PUT ALL CODE HERE

**WRITE DOCUMENTATION FOR EVERYTHING YOU WRITE**

# LEGO model Documentation

## components:
wave module 
delivery device
AUV
elevator
2 hubs

Hub 1:
submarine movement - A socket
wave module - C socket
Hub 2:
delivery device - * socket
AUV horizontal movement - E socket 
elevator (pin) - C socket
elevator (axle) - D socket
submarine deliver device - A

## Tony's naming convention (functions and syscalls):
reset - moves AUV to axle and spins out the battery (direction doesn't matter)
TOP: Wave + MOVE - moves the wave moves the bottom sub from axle to middle
Transfer: Opposite Window - transfers battery from the top to AUV (first transfer)
Transfer:Bottom: Opposite Window - transfers from AUV to Submarine
Transfer:TopLast:Opposite - puts the battery back to the top from AUV

Axle elevator only works for A and C
Pin elevator budges but doesn't do anything
#### The elevators work for A and B
Wave works
submarine movement works


# Arduino Documentation
**MOTOR1** = IN1, IN2
**MOTOR2** = IN3, IN4

MOTOR1 activates when sensor goes too much to the left
MOTOR2 activates when sensor goes too much to the right
