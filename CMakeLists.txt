cmake_minimum_required(VERSION 3.16)

project(fractalview LANGUAGES CXX)

find_package(Boost CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(fractalview
    main.cpp
    shader_load.cpp
)

target_compile_features(fractalview PRIVATE cxx_std_14)
set_target_properties(fractalview PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(fractalview PRIVATE
    Boost::headers
    GLEW::GLEW
    glfw
    OpenGL::GL
)

configure_file(mandelbulb.frag mandelbulb.frag COPYONLY)
configure_file(mandelbulb.vert mandelbulb.vert COPYONLY)
