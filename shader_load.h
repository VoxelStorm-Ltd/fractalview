#ifndef SHADER_LOAD_H_INCLUDED
#define SHADER_LOAD_H_INCLUDED

/// Macro helpers based on blob_loader.h are optional and can be disabled by
/// defining NO_BLOB_LOADER
///
/// Example of usage for a pair of files called
///   resources/shaders/entity_vert.glsl
///   resources/shaders/entity_frag.glsl
///
/// SHADER_BLOB_LOAD(entity);                     // declare the binary blobs
///
/// shader = shader_load(SHADER_BLOB(entity));
/// if(shader == GL_FALSE) {
///   std::cout << "ERROR: " << __PRETTY_FUNCTION__ << ": Failed to load shaders, exiting." << std::endl;
///   abort();
/// }

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifndef NO_BLOB_LOADER
#include "blob_loader.h"

#define SHADER_BLOB_LOAD(name) BLOB_LOAD(shaders_##name##_vert_glsl);\
                               BLOB_LOAD(shaders_##name##_frag_glsl)

#define SHADER_BLOB(name) std::string(reinterpret_cast<char const*>(BLOB(shaders_##name##_vert_glsl)),\
                                      BLOB_SIZE(                         shaders_##name##_vert_glsl)),\
                          std::string(reinterpret_cast<char const*>(BLOB(shaders_##name##_frag_glsl)),\
                                      BLOB_SIZE(                         shaders_##name##_frag_glsl))
#endif // NO_BLOB_LOADER

GLuint shader_load(std::string const &shader_vertex_source, std::string const &shader_fragment_source);

#endif // SHADER_LOAD_H_INCLUDED
