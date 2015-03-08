#include "shader_load.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

using namespace std;

GLFWwindow *window;
GLuint shader;
GLuint vao;
GLuint vbo;
GLuint window_uniform;
GLuint power_uniform;

float power = 1.0f;

static double constexpr fpscap = 60.0;
// assume we're running at a fixed 60fps
static double constexpr timestep = 1.0 / fpscap;
std::chrono::duration<double> timestep_chrono(std::chrono::milliseconds(static_cast<unsigned int>(timestep * 1000) - 1));
std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> timenexttickstart;

void glfw_error(int error, const char* description) {
  cerr << "GLFW Error: " << error << ": " << description << endl;
}

void load_shader() {
  cout << "Loading shader." << endl;

  std::stringstream fragss;
  fragss << std::ifstream("mandelbulb.frag").rdbuf();

  std::stringstream vertss;
  vertss << std::ifstream("mandelbulb.vert").rdbuf();

  shader = shader_load(vertss.str(), fragss.str());
  if(shader == 0) {
    abort();                // early exit if we don't load the shader successfully
  }
  cout << "Loaded." << endl;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);

  GLfloat vertices[] = {
      -1.0f, -1.0f,
       1.0f, -1.0f,
       1.0f,  1.0f,
      -1.0f,  1.0f
  };
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint position_attribute = glGetAttribLocation(shader, "position");
  glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(position_attribute);

  window_uniform = glGetUniformLocation(shader, "window");
  power_uniform = glGetUniformLocation(shader, "power");

  glUseProgram(shader);
}

void init_graphics() {
  if (!glfwInit()) {
    cout << "Failed to init glfw." << endl;
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(glfw_error);
  window = glfwCreateWindow(800, 600, "fractlol", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSetWindowTitle(window, "fractlol");
  glfwShowWindow(window);

#ifdef NO_VSYNC
  glfwSwapInterval(0);
#endif

  glewInit();
}

int main() {
  float fps = 0.0f;
  float ms_per_frame = 0.0f;
  double fpsinterval = 10.0;           // how many seconds between reporting performance
  int frames_last_interval = 0;
  std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> time_fpsupdate = std::chrono::high_resolution_clock::now() + std::chrono::duration<double>(std::chrono::milliseconds(static_cast<int>(1000 * fpsinterval)));


  cout << "~" << endl;
  init_graphics();
  load_shader();

  //std::this_thread::sleep_for(std::chrono::seconds(4));

  while(!glfwWindowShouldClose(window)) {
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    //std::cout << window_width << "x" << window_height << std::endl;
    glUniform2f(window_uniform, window_width, window_height);

    power += 0.01f;
    glUniform1f(power_uniform, power);
    glViewport(0, 0, window_width, window_height);
    //std::cout << power << std::endl;

    // Draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_QUADS, 0, 4);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // fps counter update
    std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<double>> const time_now(std::chrono::high_resolution_clock::now());
    // measure performance margin
    if(time_now >= time_fpsupdate) {
      fps = frames_last_interval / fpsinterval;
      ms_per_frame = 1000 * fpsinterval / frames_last_interval;
      std::cout << "ms per frame: " << ms_per_frame << " fps: " << fps << std::endl;
      time_fpsupdate = time_now + std::chrono::duration<double>(std::chrono::milliseconds(static_cast<int>(1000 * fpsinterval)));
      frames_last_interval = 0;
    } else {
      ++frames_last_interval;
    }

    // Sleep until next update. (not needed if glfw is vsyncing)
    //std::this_thread::sleep_until(timenexttickstart);
    timenexttickstart = std::chrono::high_resolution_clock::now() + timestep_chrono;
  }

  std::cout << "Cleaning up." << std::endl;
  glDeleteProgram(shader);
  glDeleteBuffers(1, &vao);
  glDeleteBuffers(1, &vbo);

  glfwTerminate();
  return EXIT_SUCCESS;
}
