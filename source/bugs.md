# Currently know bug inside the program

## Distance base collision detection
### Object clipping
#### Symptom
- Object clipping when having high velocity.
#### Possible cause
- When object travel at high velocity the travel time might become very low to the point that it got round to 0.
- Object clipping when not moving completely diagonal compare to other object.
#### Possible cause
- Need more debugging.
### Border extending bug (FIXED)
#### Symptom
- The object will extend the border to what it suppose to, but the target extend its border the wrong way.
#### Possible cause
- Poorly made border extending algorithm