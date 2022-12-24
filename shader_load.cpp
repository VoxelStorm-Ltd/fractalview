#include "shader_load.h"
#include <vector>
#include <iostream>

GLuint shader_load(std::string const &shader_vertex_source, std::string const &shader_fragment_source) {
  /// Load a fragment and vertex shader from a string and return the compiled shader program
  char const *shader_vert_src_c = shader_vertex_source.c_str();
  char const *shader_frag_src_c = shader_fragment_source.c_str();

  GLuint shader_vert    = 0;                                                    // vertex shader
  GLuint shader_frag    = 0;                                                    // fragment shader
  GLuint shader_program = 0;                                                    // linked shader program
  shader_vert = glCreateShader(GL_VERTEX_SHADER);
  shader_frag = glCreateShader(GL_FRAGMENT_SHADER);

  // Compile vertex shader
  #ifdef DEBUG_SHADER_LOAD_INFO
    std::cout << "Shader: Compiling vertex... ";
  #endif // DEBUG_SHADER_LOAD_INFO
  glShaderSource(shader_vert, 1, &shader_vert_src_c, NULL);
  glCompileShader(shader_vert);

  auto detach_all = [&]{
    glDetachShader(shader_program, shader_vert);                                // glDeleteShader won't work until the shaders have been detached
    glDetachShader(shader_program, shader_frag);
    glDeleteShader(shader_vert);
    glDeleteShader(shader_frag);
  };

  {
    // Check vertex shader
    GLint result = GL_FALSE;
    glGetShaderiv(shader_vert, GL_COMPILE_STATUS, &result);
    if(result == GL_TRUE) {
      //std::cout << "done (" << shader_vert << ")." << std::endl;
    } else {
      GLint log_length = 0;
      glGetShaderiv(shader_vert, GL_INFO_LOG_LENGTH, &log_length);
      #ifdef DEBUG_SHADER_LOAD_EXTRA
        std::cout << "[DEBUG: vert GL_COMPILE_STATUS " << result << " GL_INFO_LOG_LENGTH " << log_length << "] ";
      #endif // DEBUG_SHADER_LOAD_EXTRA
      #ifdef DEBUG_SHADER_LOAD_ERROR
        #ifndef DEBUG_SHADER_LOAD_INFO
          std::cout << "Shader: ERROR: Compiling vertex shader ";
        #endif // DEBUG_SHADER_LOAD_INFO
        std::cout << "failed, code " << result;
      #endif // DEBUG_SHADER_LOAD_ERROR
      if(log_length > 0) {
        std::vector<char> shader_vert_error(static_cast<size_t>(log_length));
        glGetShaderInfoLog(shader_vert, log_length, NULL, &shader_vert_error[0]); // only try to get a log if one is available
        #ifdef DEBUG_SHADER_LOAD_ERROR
          std::cout << ":" << std::endl << &shader_vert_error[0] << std::endl;
        #endif // DEBUG_SHADER_LOAD_ERROR
      } else {
        #ifdef DEBUG_SHADER_LOAD_ERROR
          std::cout << "." << std::endl;
        #endif // DEBUG_SHADER_LOAD_ERROR
      }
      detach_all();
      return GL_FALSE;
    }
  }

  // Compile fragment shader
  #ifdef DEBUG_SHADER_LOAD_INFO
    std::cout << "fragment... ";
  #endif // DEBUG_SHADER_LOAD_INFO
  glShaderSource(shader_frag, 1, &shader_frag_src_c, NULL);
  glCompileShader(shader_frag);
  {
    // Check fragment shader
    GLint result = GL_FALSE;
    glGetShaderiv(shader_frag, GL_COMPILE_STATUS, &result);
    if(result == GL_TRUE) {
      //std::cout << "done (" << shader_frag << ")." << std::endl;
    } else {
      GLint log_length = 0;
      glGetShaderiv(shader_frag, GL_INFO_LOG_LENGTH, &log_length);
      #ifdef DEBUG_SHADER_LOAD_EXTRA
        std::cout << "[DEBUG: frag GL_COMPILE_STATUS " << result << " GL_INFO_LOG_LENGTH " << log_length << "] ";
      #endif // DEBUG_SHADER_LOAD_EXTRA
      #ifdef DEBUG_SHADER_LOAD_ERROR
        #ifndef DEBUG_SHADER_LOAD_INFO
          std::cout << "Shader: ERROR: Compiling fragment shader ";
        #endif // DEBUG_SHADER_LOAD_INFO
        std::cout << "failed, code " << result;
      #endif // DEBUG_SHADER_LOAD_ERROR
      if(log_length > 0) {
        std::vector<char> shader_frag_error(static_cast<size_t>(log_length));
        glGetShaderInfoLog(shader_frag, log_length, NULL, &shader_frag_error[0]); // only try to get a log if one is available
        #ifdef DEBUG_SHADER_LOAD_ERROR
          std::cout << result << ":" << std::endl << &shader_frag_error[0] << std::endl;
        #endif // DEBUG_SHADER_LOAD_ERROR
      } else {
        #ifdef DEBUG_SHADER_LOAD_ERROR
          std::cout << result << "." << std::endl;
        #endif // DEBUG_SHADER_LOAD_ERROR
      }
      detach_all();
      return GL_FALSE;
    }
  }

  // Link the shader program
  #ifdef DEBUG_SHADER_LOAD_INFO
    std::cout << "linking... ";
  #endif // DEBUG_SHADER_LOAD_INFO
  shader_program = glCreateProgram();                                           // create the shader program
  glAttachShader(shader_program, shader_vert);
  glAttachShader(shader_program, shader_frag);
  glLinkProgram(shader_program);
  {
    // Check link success
    GLint result = GL_FALSE;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
    if(result == GL_TRUE) {
      std::cout << "done (" << shader_program << ")." << std::endl;
    } else {
      GLint log_length = 0;
      glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
      #ifdef DEBUG_SHADER_LOAD_EXTRA
        std::cout << "[DEBUG: link GL_COMPILE_STATUS " << result << " GL_INFO_LOG_LENGTH " << log_length << "] ";
      #endif // DEBUG_SHADER_LOAD_EXTRA
      #ifdef DEBUG_SHADER_LOAD_ERROR
        #ifndef DEBUG_SHADER_LOAD_INFO
          std::cout << "Shader: ERROR: Linking shader ";
        #endif // DEBUG_SHADER_LOAD_INFO
        std::cout << "failed, code " << result;
      #endif // DEBUG_SHADER_LOAD_ERROR
      if(log_length > 0) {
        std::vector<char> program_error(static_cast<size_t>(log_length));
        glGetProgramInfoLog(shader_program, log_length, NULL, &program_error[0]); // only try to get a log if one is available
        #ifdef DEBUG_SHADER_LOAD_ERROR
          std::cout << ":" << std::endl << &program_error[0] << std::endl;
        #endif // DEBUG_SHADER_LOAD_ERROR
      } else {
        #ifdef DEBUG_SHADER_LOAD_ERROR
          std::cout << "." << std::endl;
        #endif // DEBUG_SHADER_LOAD_ERROR
      }
      detach_all();
      return GL_FALSE;
    }
  }

  detach_all();
  return shader_program;
}
