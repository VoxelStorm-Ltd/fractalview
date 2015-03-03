#ifndef SHADER_LOAD_H_INCLUDED
#define SHADER_LOAD_H_INCLUDED

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint shader_load(std::string const &shader_vertex_source, std::string const &shader_fragment_source);

#endif // SHADER_LOAD_H_INCLUDED
