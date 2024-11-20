# Currently know bug inside the program

## Object clipping
- Possible cause
    - Object hitting another moving object may result in clipping or weird collision result idk
    - Object hitting another two object in short amount of time
    - When running the program for the first time, the first deltaTime maybe a bit too high (Only happen half the time)

## Two object moving on each other and bounce in the wrong way
- Possible cause
    - Need more debugging but definitely happen inside object ram into border logic