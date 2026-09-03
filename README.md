# fractalview

A small OpenGL fractal viewer.

## Building

The project requires CMake 3.16 or newer, a C++14 compiler, and development
packages for Boost, GLEW, GLFW, and OpenGL.

Configure and build it from the repository root:

```sh
cmake -S . -B build
cmake --build build
```

Run the executable from the build directory so it can find the copied shader
files:

```sh
cd build
./fractalview
```
