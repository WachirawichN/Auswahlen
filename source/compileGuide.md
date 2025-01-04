# Compiling guide
## Windows 11 (MSVC)
Step 1. run 'vcvars64' on developer command prompt to change the comepile mode to x64.
Step 2. Compile the CUDA code using 'nvcc -c addVector.cu -o addVector.obj -Xcompiler "/MD"'.
Step 3. Compile the C++ code into .exe file using 'cl /EHsc /MD /D GLEW_STATIC main.cpp addVector.obj /link /LIBPATH:"G:\Code_saves\Auswahlen\source\dependencies\GLFW" /LIBPATH:"G:\Code_saves\Auswahlen\source\dependencies\GLEW\lib\Release\x64" cudart.lib glew32s.lib glfw3.lib opengl32.lib User32.lib Gdi32.lib Shell32.lib'


## Compile CUDA file to obj
```
nvcc simulation/simulation.cu simulation/object/objectBaseClass.cu simulation/physic/fundamental.cu simulation/physic/collision.cu simulation/physic/momentum.cu simulation/physic/hitbox.cu simulation/utility/mathExt.cu simulation/utility/cudaUtil.cu -o test.obj -Xcompiler "/MD"
```
```
nvcc simulation/simulation.cu simulation/object/objectBaseClass.cu simulation/physic/fundamental.cu  simulation/utility/cudaUtil.cu -o test.obj -Xcompiler "/MD"
```

## Compile project
```
cl /EHsc /MD /D GLEW_STATIC main.cpp graphic/indexBuffer.cpp graphic/renderer.cpp graphic/vertexArray.cpp graphic/vertexBuffer.cpp graphic/vertexBufferLayout.cpp graphic/shader.cpp graphic/texture.cpp graphic/camera.cpp graphic/geometry/cube.cpp graphic/geometry/pyramid.cpp graphic/geometry/icosphere.cpp dependencies/stb/stb_image.cpp /link /LIBPATH:"dependencies\GLFW" /LIBPATH:"dependencies\GLEW\lib\Release\x64" glew32s.lib glfw3.lib opengl32.lib User32.lib Gdi32.lib Shell32.lib
```

## Compile linear Algebra library
```
cl /EHsc /MD /D GLEW_STATIC test.cpp vector/indexProxy.cpp vector/vec3.cpp vector/vec4.cpp User32.lib Gdi32.lib Shell32.lib
```