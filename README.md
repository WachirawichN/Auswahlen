# Auswählen

## About
A physic engine powered by CUDA. Focus on simulating simulation that benefit from parallel computing, for example fluid simulation, aerodynamic simulation, etc.

## Changelog
### v.0.1.0
- Add a working simulation with
    1. Rendering system.
    2. Manual physic stepping, going forward / backward in time or even pause physic calculation.
    3. Camera system with movement.
    4. Gravity.
    5. Collision detection.
    6. Elastic collision.
- All of the function above still cannot run on GPU, literally just a prototype that run exclusively on CPU for now.