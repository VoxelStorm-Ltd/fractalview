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

#define DEBUG_SHADER_LOAD_INFO                                                  // informational messages to stdout
#define DEBUG_SHADER_LOAD_ERROR                                                 // error messages to stdout
//#define DEBUG_SHADER_LOAD_EXTRA                                                 // extra debugging info to stdout

#include <string>
#ifndef __EMSCRIPTEN__
  #include <GL/glew.h>
#endif // __EMSCRIPTEN__
#include <GL/gl.h>
#ifndef NO_BLOB_LOADER
#include "blob_loader.h"

#define SHADER_BLOB_LOAD(name) BLOB_LOAD(shaders_##name##_vert_glsl);\
                               BLOB_LOAD(shaders_##name##_frag_glsl)

#define SHADER_BLOB(name) STRING_BLOB(shaders_##name##_vert_glsl), \
                          STRING_BLOB(shaders_##name##_frag_glsl)

#endif // NO_BLOB_LOADER

GLuint shader_load(std::string const &shader_vertex_source, std::string const &shader_fragment_source);
GLuint shader_load(char const *shader_vertex_source, char const *shader_fragment_source);

#endif // SHADER_LOAD_H_INCLUDED
