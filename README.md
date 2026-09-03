# fractalview

[![Build](https://github.com/VoxelStorm-Ltd/fractalview/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/VoxelStorm-Ltd/fractalview/actions/workflows/cmake-single-platform.yml)

A small OpenGL fractal viewer.

<img width="800" height="633" alt="image" src="https://github.com/user-attachments/assets/b2d8031b-55bf-4d92-908f-0c932c0dde93" /> <img width="800" height="633" alt="image" src="https://github.com/user-attachments/assets/a5431473-4b0c-471f-8e26-ae45ed533cda" />

## Controls

| Input | Action |
| --- | --- |
| Mouse | Look around |
| <kbd>W</kbd> / <kbd>S</kbd> | Move forward / backward |
| <kbd>A</kbd> / <kbd>D</kbd> | Move left / right |
| <kbd>Space</kbd> / <kbd>Left Ctrl</kbd> | Move up / down |
| <kbd>1</kbd> | Halve movement speed |
| <kbd>2</kbd> | Double movement speed |

## Building

The project requires CMake 3.20 or newer, a C++23 compiler, and development
packages for Boost, GLEW, GLFW, and OpenGL. The vectorstorm maths library is
included in the source tree.

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
