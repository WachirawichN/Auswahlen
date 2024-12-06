# Currently know bug inside the program

## Distance base collision detection
### Symptom
- Object clipping
### Possible cause
- After the object collision has been resolved from one object pair, the target object velocity will be change but the time to calculate the new position will still be delta time which will mostly be too much.
- The formular for calculating the time for the object and the target to hit each other is wrong.
    - When the object and the target is moving in the opposite direction, the formular still count it as the same direction.
    - Using time that come from pythagoras to calculate the distance the object / target need to move, the distance may be a bit too much.

## Manual physic stepping
### Symptom
- No collision detection when stepping backward
### Possible cause
- Currently unknown