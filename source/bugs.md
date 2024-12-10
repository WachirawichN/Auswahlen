# Currently know bug inside the program

## Distance base collision detection
### Object clipping
#### Symptom
- Object clipping when having high velocity.
- Object clipping when moving in only 1 axis.
#### Possible cause
- Need more debugging.
### Border extending bug (FIXED)
#### Symptom
- The object will extend the border to what it suppose to, but the target extend its border the wrong way.
#### Possible cause
- Poorly made border extending algorithm