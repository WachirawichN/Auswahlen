# To do list
List of feature that will be added in the future. Higher ranking = higher priority.

- Add parallelism. (Release v.1.0.0)
    - Make the homemade linear algebra library fully run on CUDA.
    - Completely move from GLM to homemade linear algebra library.
    - CUDA cannot run GLM on the GPU.
- Optimize the project.
    - After moving from MSVC there's a significant performance loss after moving.
- Fix mysterious clipping bug.
- Dedicated sphere hitbox.
- Hitbox calculation for object that has been rotate.
- Angular momentum.
- Finish unbalance time to move function.
- Control for changing object position / scale / velocity mid simulation.
- Object transparency.