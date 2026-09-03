#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numbers>
#include <sstream>
#include <streambuf>
#include <string>
#include <thread>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vectorstorm/quat/quat.h>
#include <vectorstorm/vector/vector3.h>
#include "shader_load.h"

using namespace std;

GLFWwindow *window;
GLuint shader;
GLuint vao;
GLuint vbo;
GLuint window_uniform;
GLuint cam_pos_uniform;
GLuint cam_dir_uniform;
GLuint cam_up_uniform;
GLuint cam_side_uniform;
GLuint julia_uniform;

float power = 1.0f;

bool move_forward = false;
bool move_back = false;
bool move_left = false;
bool move_right = false;
bool move_up = false;
bool move_down = false;

quatf heading;
float yaw{0.0f};
float pitch{0.0f};
double previous_cursor_x{0.0};
double previous_cursor_y{0.0};
bool received_cursor_position{false};

float constexpr mouse_sensitivity{0.0025f};
float constexpr pitch_limit{std::numbers::pi_v<float> / 2.0f - 0.01f};

vec3f cam_pos = {-0.0f, 3.14f, -2.0f};
vec3f cam_dir = {0.0f, 0.0f, 0.0f};
vec3f cam_up = {0.0f, 1.0f, 0.0f};
vec3f julia = {1.0f, 1.0f, 0.95f};

float speed = 0.01f;

static double constexpr fpscap = 60.0;
// assume we're running at a fixed 60fps
static double constexpr timestep = 1.0 / fpscap;
chrono::duration<double> timestep_chrono(chrono::milliseconds(static_cast<unsigned int>(timestep * 1000) - 1));
chrono::time_point<chrono::high_resolution_clock, chrono::duration<double>> timenexttickstart;

void glfw_error(int error, const char* description) {
  cerr << "GLFW Error: " << error << ": " << description << endl;
}

void load_shader() {
  cout << "Loading shader." << endl;

  stringstream fragss;
  fragss << ifstream("mandelbulb.frag").rdbuf();

  stringstream vertss;
  vertss << ifstream("mandelbulb.vert").rdbuf();

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
  cam_up_uniform = glGetUniformLocation(shader, "camUp");
  cam_side_uniform = glGetUniformLocation(shader, "camSide");
  julia_uniform = glGetUniformLocation(shader, "julia");

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
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if(glfwRawMouseMotionSupported() == GLFW_TRUE) {
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }
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
    else if (key == GLFW_KEY_SPACE) move_up = true;
    else if (key == GLFW_KEY_LEFT_CONTROL) move_down = true;
    else if (key == GLFW_KEY_1) speed /= 2.0f;
    else if (key == GLFW_KEY_2) speed *= 2.0f;
  } else if (action == GLFW_RELEASE) {
    if (key == GLFW_KEY_W) move_forward = false;
    else if (key == GLFW_KEY_S) move_back = false;
    else if (key == GLFW_KEY_A) move_left = false;
    else if (key == GLFW_KEY_D) move_right = false;
    else if (key == GLFW_KEY_SPACE) move_up = false;
    else if (key == GLFW_KEY_LEFT_CONTROL) move_down = false;
  }
}

static void cursor_pos_callback(GLFWwindow *, double const cursor_x, double const cursor_y) {
  if(!received_cursor_position) {
    previous_cursor_x = cursor_x;
    previous_cursor_y = cursor_y;
    received_cursor_position = true;
    return;
  }

  double const delta_x{cursor_x - previous_cursor_x};
  double const delta_y{cursor_y - previous_cursor_y};
  previous_cursor_x = cursor_x;
  previous_cursor_y = cursor_y;

  yaw = std::remainder(yaw - static_cast<float>(delta_x) * mouse_sensitivity,
                       2.0f * std::numbers::pi_v<float>);
  pitch = std::clamp(pitch - static_cast<float>(delta_y) * mouse_sensitivity,
                     -pitch_limit,
                     pitch_limit);
  heading = quatf::from_euler_angles_rad(0.0f, yaw, 0.0f)
          * quatf::from_euler_angles_rad(0.0f, 0.0f, pitch);
}

static void window_focus_callback(GLFWwindow *, int const focused) {
  if(focused == GLFW_TRUE) received_cursor_position = false;
}

int main() {
  float fps = 0.0f;
  float ms_per_frame = 0.0f;
  double fpsinterval = 10.0;           // how many seconds between reporting performance
  int frames_last_interval = 0;
  chrono::time_point<chrono::high_resolution_clock, chrono::duration<double>> time_fpsupdate = chrono::high_resolution_clock::now() + chrono::duration<double>(chrono::milliseconds(static_cast<int>(1000 * fpsinterval)));


  cout << "~" << endl;
  init_graphics();
  load_shader();

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_pos_callback);
  glfwSetWindowFocusCallback(window, window_focus_callback);

  //this_thread::sleep_for(chrono::seconds(4));

  while(!glfwWindowShouldClose(window)) {
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    //cout << window_width << "x" << window_height << endl;

    cam_dir = vec3f(1.0f, 0.0f, 0.0f);
    cam_dir.rotate(heading);
    cam_up = vec3f(0.0f, 1.0f, 0.0f);
    cam_up.rotate(heading);

    //cout << "Cam dir: " << cam_dir << endl;
    //cout << "Cam up: " << cam_up << endl;

    vec3f cam_right = cam_dir.cross(cam_up);

    if (move_forward) cam_pos += cam_dir*speed;
    if (move_back) cam_pos -= cam_dir*speed;
    if (move_right) cam_pos += cam_right*speed;
    if (move_left) cam_pos -= cam_right*speed;
    if (move_up) cam_pos += cam_up*speed;
    if (move_down) cam_pos -= cam_up*speed;

    /*if (move_forward) julia += cam_dir/100000.0;
    if (move_back) julia -= cam_dir/100000.0;
    if (move_right) julia += cam_right/100000.0;
    if (move_left) julia -= cam_right/100000.0;
    if (move_up) julia += cam_up/100000.0;
    if (move_down) julia -= cam_up/100000.0;*/

    glUniform2f(window_uniform, window_width, window_height);
    glUniform3f(cam_pos_uniform, cam_pos[0], cam_pos[1], cam_pos[2]);
    glUniform3f(cam_dir_uniform, cam_dir[0], cam_dir[1], cam_dir[2]);
    glUniform3f(cam_up_uniform, cam_up[0], cam_up[1], cam_up[2]);
    glUniform3f(cam_side_uniform, cam_right[0], cam_right[1], cam_right[2]);
    glUniform3f(julia_uniform, julia[0], julia[1], julia[2]);
    glViewport(0, 0, window_width, window_height);
    //cout << "Window: " << window_width << "x" << window_height << endl;
    //cout << "Pos: " << cam_pos[0] << "," << cam_pos[1] << "," << cam_pos[2] << endl;
    //cout << "Dir: " << cam_dir[0] << "," << cam_dir[1] << "," << cam_dir[2] << endl;

    // Draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_QUADS, 0, 4);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // fps counter update
    chrono::time_point<chrono::high_resolution_clock, chrono::duration<double>> const time_now(chrono::high_resolution_clock::now());
    // measure performance margin
    if(time_now >= time_fpsupdate) {
      fps = frames_last_interval / fpsinterval;
      ms_per_frame = 1000 * fpsinterval / frames_last_interval;
      cout << "ms per frame: " << ms_per_frame << " fps: " << fps << endl;
      time_fpsupdate = time_now + chrono::duration<double>(chrono::milliseconds(static_cast<int>(1000 * fpsinterval)));
      frames_last_interval = 0;
    } else {
      ++frames_last_interval;
    }

    // Sleep until next update. (not needed if glfw is vsyncing)
    //this_thread::sleep_until(timenexttickstart);
    timenexttickstart = chrono::high_resolution_clock::now() + timestep_chrono;
  }

  cout << "Cleaning up." << endl;
  glDeleteProgram(shader);
  glDeleteBuffers(1, &vao);
  glDeleteBuffers(1, &vbo);

  glfwTerminate();
  return EXIT_SUCCESS;
}
