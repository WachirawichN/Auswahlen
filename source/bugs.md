# Currently know bug inside the program

## Continuous collision detection
### Object clipping
- Object clipping when having high velocity.
    - When object travel at high velocity the travel time might become very low to the point that it got round to 0.
- Object clipping when slower object hit other object with faster velocity in the back when both object's velocity go to same direction will cause the elastic collision to become unusable.
    - Using pythagoras did help but sometime cause a lot of lag, which is probably due to the object ram into each other multiple time before breaking free from each other.
    - Happen when object and target hit each other in the corner with uneven velocity on each axis.
    - Impliment a function that turn some momentum in to angular momentum might help.
    - Did not happen when the manual step delta time is 0.001.
- Object clipping with the box that the object is inside of.
    - Problem with object border selection when the target velocity is 0.

## Updating simulation
### Manualy update simulation / Automaticly update simulation
- Using a big step may have a goofy effect with how the collision detection work.
    - adding a sub step may help.