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
GLuint cam_pos_uniform;
GLuint cam_dir_uniform;

float power = 1.0f;

bool move_forward = false;
bool move_back = false;
bool move_left = false;
bool move_right = false;

GLfloat cam_pos[] = {0.0f, 10.0f, 0.0f};
GLfloat cam_dir[] = {0.0f, -1.0f, -1.0f};

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
  cam_pos_uniform = glGetUniformLocation(shader, "camPos");
  cam_dir_uniform = glGetUniformLocation(shader, "camDir");

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_W) move_forward = true;
    else if (key == GLFW_KEY_S) move_back = true;
    else if (key == GLFW_KEY_A) move_left = true;
    else if (key == GLFW_KEY_D) move_right = true;
  } else if (action == GLFW_RELEASE) {
    if (key == GLFW_KEY_W) move_forward = false;
    else if (key == GLFW_KEY_S) move_back = false;
    else if (key == GLFW_KEY_A) move_left = false;
    else if (key == GLFW_KEY_D) move_right = false;
  }
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
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
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_pos_callback);

  //std::this_thread::sleep_for(std::chrono::seconds(4));

  while(!glfwWindowShouldClose(window)) {
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    //std::cout << window_width << "x" << window_height << std::endl;

    if (move_forward) {
      cam_pos[0] += 0.1f;
    } else if (move_back) {
      cam_pos[0] -= 0.1f;
    } else if (move_right) {
      cam_pos[2] += 0.1f;
    } else if (move_left) {
      cam_pos[2] -= 0.1f;
    }

    glUniform2f(window_uniform, window_width, window_height);
    glUniform3f(cam_pos_uniform, cam_pos[0], cam_pos[1], cam_pos[2]);
    glUniform3f(cam_dir_uniform, cam_dir[0], cam_dir[1], cam_dir[2]);
    glViewport(0, 0, window_width, window_height);
    std::cout << "Window: " << window_width << "x" << window_height << std::endl;
    std::cout << "Pos: " << cam_pos[0] << "," << cam_pos[1] << "," << cam_pos[2] << std::endl;
    std::cout << "Dir: " << cam_dir[0] << "," << cam_dir[1] << "," << cam_dir[2] << std::endl;

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

