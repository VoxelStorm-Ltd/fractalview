#include "shader_load.h"
#include <vector>
#include <iostream>

GLuint shader_load(std::string const &shader_vertex_source, std::string const &shader_fragment_source) {
  /// Load a fragment and vertex shader from a string and return the compiled shader program
  char const *shader_vert_src_c = shader_vertex_source.c_str();
  char const *shader_frag_src_c = shader_fragment_source.c_str();

  GLuint shader_vert = 0;   // vertex shader
  GLuint shader_frag = 0;   // fragment shader
  GLuint shader_program = 0;   // linked shader program
  shader_vert = glCreateShader(GL_VERTEX_SHADER);
  shader_frag = glCreateShader(GL_FRAGMENT_SHADER);

  // Compile vertex shader
  //std::cout << "Shader: Compiling vertex shader...   ";
  std::cout << "Shader: Compiling vertex... ";
  glShaderSource(shader_vert, 1, &shader_vert_src_c, NULL);
  glCompileShader(shader_vert);

  // Check vertex shader
  GLint result = GL_FALSE;
  GLint log_length = 0;
  glGetShaderiv(shader_vert, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_vert, GL_INFO_LOG_LENGTH, &log_length);
  std::vector<char> shader_vert_error(log_length);
  glGetShaderInfoLog(shader_vert, log_length, NULL, &shader_vert_error[0]);
  if(result == GL_TRUE) {
    //std::cout << "done (" << shader_vert << ")." << std::endl;
  } else {
    std::cout << "failed, code " << result << ":" << std::endl;
    std::cout << &shader_vert_error[0] << std::endl;
    glDetachShader(shader_program, shader_vert);    // glDeleteShader won't work until the shaders have been detached
    glDetachShader(shader_program, shader_frag);
    glDeleteShader(shader_vert);
    glDeleteShader(shader_frag);
    return GL_FALSE;
  }

  // Compile fragment shader
  //std::cout << "Shader: Compiling fragment shader... ";
  std::cout << "fragment... ";
  glShaderSource(shader_frag, 1, &shader_frag_src_c, NULL);
  glCompileShader(shader_frag);

  // Check fragment shader
  glGetShaderiv(shader_frag, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_frag, GL_INFO_LOG_LENGTH, &log_length);
  std::vector<char> shader_frag_error(log_length);
  glGetShaderInfoLog(shader_frag, log_length, NULL, &shader_frag_error[0]);
  if(result == GL_TRUE) {
    //std::cout << "done (" << shader_frag << ")." << std::endl;
  } else {
    std::cout << "failed, code " << result << ":" << std::endl;
    std::cout << &shader_frag_error[0] << std::endl;
    glDetachShader(shader_program, shader_vert);    // glDeleteShader won't work until the shaders have been detached
    glDetachShader(shader_program, shader_frag);
    glDeleteShader(shader_vert);
    glDeleteShader(shader_frag);
    return GL_FALSE;
  }

  //std::cout << "Shader: Linking program...           ";
  std::cout << "linking... ";
  shader_program = glCreateProgram();
  glAttachShader(shader_program, shader_vert);
  glAttachShader(shader_program, shader_frag);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
  glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
  std::vector<char> programError( (log_length > 1) ? log_length : 1 );
  glGetProgramInfoLog(shader_program, log_length, NULL, &programError[0]);
  if(result == GL_TRUE) {
    std::cout << "done (" << shader_program << ")." << std::endl;
  } else {
    std::cout << "failed, code " << result << ":" << std::endl;
    std::cout << &programError[0] << std::endl;
    glDetachShader(shader_program, shader_vert);    // glDeleteShader won't work until the shaders have been detached
    glDetachShader(shader_program, shader_frag);
    glDeleteShader(shader_vert);
    glDeleteShader(shader_frag);
    return GL_FALSE;
  }

  glDetachShader(shader_program, shader_vert);    // glDeleteShader won't work until the shaders have been detached
  glDetachShader(shader_program, shader_frag);
  glDeleteShader(shader_vert);
  glDeleteShader(shader_frag);

  return shader_program;
}
