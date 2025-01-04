# To do list
List of feature that will be added in the future. Higher ranking is higher priority.

- Add parallelism.
    - Make apply gravity usable will already be enough, because moving from GLM to homemade library will be a big problem that need to fix very soon.
- Use Cmake instead of MSVC compiler.
- Completely move from GLM to homemade linear algebra library.
    - CUDA cannot run GLM on the GPU.
- Make the homemade linear algebra library fully run on CUDA.
- Fix mysterious clipping bug.
- Dedicated sphere hitbox.
- Hitbox calculation for object that has been rotate.
- Finish unbalance time to move function.
- Control for changing object position / scale / velocity mid simulation.
- Object transparency.