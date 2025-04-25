# Currently know bug inside the program

## Continuous collision detection
### Object clipping
- Object clipping, no information about this has been found yet but has been occur before.
    - Status: Not fix. No information about it.
    - May cause by object collide with two different object at the same frame, but using a wrong order of checking collision maybe.
    - Have higher tendency to occure when running at lower FPS.
- Object clipping when having high velocity.
    - Status: Not fix.
    - When object travel at high velocity the travel time might become very low to the point that it got round to 0.
- Object hit each other in the corner have a chance to clip each other.
    - Status: **FIXED.** (But resolving part might not be very accurate.)
    - Using lowest time to move object isn't the best way to resolve the collision because if the object collide with each other in more than one axis, the axis with higher time will still not hit the other object in that axis when the collision have been resolve.
    - Sometime the pythagoras time use to move object before resolved the velocity is too high, but sometimes it is perfect.
- Object clipping with the box that the object is inside of.
    - Status: **FIXED.**
    - Problem with object border selection when the target velocity is 0.

## Updating simulation
### Manualy update simulation / Automaticly update simulation
- Using a big step may have a goofy effect with how the collision detection work.
    - Status: Not fix.
    - adding a sub step may help.