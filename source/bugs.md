# Currently know bug inside the program

## Distance base collision detection
### Object clipping
- Object clipping when having high velocity.
    - When object travel at high velocity the travel time might become very low to the point that it got round to 0.
- Falsely calculated collision type when time is going backward. (fixed)
    - Multiply velocity with time multiplier will cause the travel time function to calculated the time use to collide wrong.
