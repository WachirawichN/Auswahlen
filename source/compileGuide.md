Windows 11 (MSVC)
Step 1. run 'vcvars64' on developer command prompt to change the comepile mode to x64.
Step 2. Compile the CUDA code using 'nvcc -c addVector.cu -o addVector.obj -Xcompiler "/MD"'.
Step 3. Compile the C++ code into .exe file using 'cl /EHsc /MD /D GLEW_STATIC main.cpp addVector.obj /link /LIBPATH:"G:\Code_saves\Auswahlen\source\dependencies\GLFW" /LIBPATH:"G:\Code_saves\Auswahlen\source\dependencies\GLEW\lib\Release\x64" cudart.lib glew32s.lib glfw3.lib opengl32.lib User32.lib Gdi32.lib Shell32.lib'




cl /EHsc /MD /D GLEW_STATIC main.cpp graphic/indexBuffer.cpp graphic/renderer.cpp graphic/vertexArray.cpp graphic/vertexBuffer.cpp graphic/vertexBufferLayout.cpp graphic/shader.cpp graphic/texture.cpp graphic/camera.cpp graphic/geometry/cube.cpp graphic/geometry/pyramid.cpp graphic/geometry/icosphere.cpp simulation/simulation.cpp simulation/object/objectBaseClass.cpp dependencies/stb/stb_image.cpp /link /LIBPATH:"dependencies\GLFW" /LIBPATH:"dependencies\GLEW\lib\Release\x64" glew32s.lib glfw3.lib opengl32.lib User32.lib Gdi32.lib Shell32.lib