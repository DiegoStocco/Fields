#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "errors.h"
#include "field.h"

JAGE::Window* init()
{
  // Initialize the library
  if (!glfwInit())
    std::cerr << "Error in initializing GLFW\n";

  JAGE::enable_glfw_errors();

  JAGE::Window *window = new JAGE::Window("Field", 900, 700);
  window->Bind();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Init GLEW to import opengl functions
  if(glewInit() != GLEW_OK)
    std::cerr << "Error in GLEW initialization\n";

  JAGE::enable_gl_errors();

  // Print openGL version
  std::cout << glGetString(GL_VERSION) << "\n";

  return window;
}

int main(int argc, char* argv[]) {
  JAGE::Window* window = init();

  Field field;
  float lastTime = glfwGetTime(), currTime;
  while(!window->shouldClose()) {
    currTime = glfwGetTime();
    field.update(currTime - lastTime);
    field.render(window);
    lastTime = currTime;
  }
}
