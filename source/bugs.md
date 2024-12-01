# Currently know bug inside the program

## Distance base collision detection
### Symptom
- Object clipping
### Possible cause
- The formular for calculating the time for the object and the target to hit each other is wrong.
    - When the object and the target is moving in the opposite direction, the formular still count it as the same direction.

## Manual physic stepping
### Symptom
- No collision detection when stepping backward
### Possible cause
- Currently unknown