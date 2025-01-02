# Currently know bug inside the program

## Continuous collision detection
### Object clipping
- Object clipping when having high velocity.
    - Status: Not fix.
    - When object travel at high velocity the travel time might become very low to the point that it got round to 0.
- Object hit each other in the corner have a chance to clip each other.
    - Status: Some what fix (Sometimes still occured).
    - Sometime the pythagoras time use to move object before resolved the velocity is too high, but sometimes it is perfect.
- **FIXED**: Object clipping with the box that the object is inside of.
    - Status: **FIXED.**
    - Problem with object border selection when the target velocity is 0.

## Updating simulation
### Manualy update simulation / Automaticly update simulation
- Using a big step may have a goofy effect with how the collision detection work.
    - Status: Not fix.
    - adding a sub step may help.