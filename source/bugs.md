# Currently know bug inside the program

## Continuous collision detection
### Object clipping
- Object clipping when having high velocity.
    - When object travel at high velocity the travel time might become very low to the point that it got round to 0.
- Object clipping when slower object hit other object with faster velocity in the back when both object's velocity go to same direction will cause the elastic collision to become unusable.
    - Impliment a function that turn some momentum in to angular momentum might help.
    - Happen mostly when faster object try to surpass slower object in that axis.
- Object clipping when step the simulation manualy with very high delta time.

## Updating simulation
### Manualy update simulation / Automaticly update simulation
- Somehow manualy updating will result in different result compare to automaticly updating.