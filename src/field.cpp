#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <window.h>
#include <renderer.h>
#include <shader_storage_buffer.h>
#include "field.hpp"
#include "vertex_buffer.h"

Field::Field(unsigned int nOfBodies, JAGE::Window* window) {
  m_nBodies = nOfBodies;
  m_window = window;

  // Initialize bodies positions and velocity
  glm::vec4* initial_pos = new glm::vec4[nOfBodies];
  unsigned int grid[2] = {nOfBodies/(unsigned)sqrt(nOfBodies), (unsigned)sqrt(nOfBodies)};
  for(int i = 0; i < nOfBodies; i++)
    initial_pos[i] = 2.0f*glm::vec4(float(i%grid[0])/grid[0] , float(i/grid[1])/grid[1],0,0) - glm::vec4(1,1,0,0);

  m_bodiesPos = new JAGE::ShaderStorageBuffer(initial_pos, nOfBodies*sizeof(glm::vec4), GL_DYNAMIC_COPY);

  delete[] initial_pos;

  // Load shaders
  m_renderProgram = new JAGE::Shader({{"shaders/vertex_shader.glsl", GL_VERTEX_SHADER}, {"shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER}});
  m_updateBodiesProgram = new JAGE::Shader({{"shaders/update_bodies.glsl", GL_COMPUTE_SHADER}});

  // Triangles where to draw the field (whole screen)
  float triangles_pos[2*4] = {
    -1.0f,  1.0f,
     1.0f,  1.0f,
     1.0f, -1.0f,
    -1.0f, -1.0f,
  };
  static JAGE::VertexBuffer triangles_buf(triangles_pos, sizeof(triangles_pos));
  JAGE::VertexBufferLayout layout;
  layout.Push<float>(2);

  m_triangles = new JAGE::VertexArray;
  m_triangles->AddBuffer(triangles_buf, layout);

  unsigned int triangles_indicies[2*3] = {
    0, 1, 2,
    2, 3, 0,
  };
  m_triangles_indicies = new JAGE::IndexBuffer(triangles_indicies, 6);

}

Field::~Field() {
  delete m_bodiesPos;
  delete m_renderProgram;
  delete m_updateBodiesProgram;
  delete m_triangles;
  delete m_triangles_indicies;
}

void Field::update(float deltaTime) {
  glm::vec2 mouse_pos = glm::vec2(2,2) * m_window->getMousePos() / glm::vec2(m_window->getWidth(), m_window->getHeight()) - glm::vec2(1,1);

  // The mouse position given by glfw is inverted
  mouse_pos.y *= -1;

  m_bodiesPos->Bind(2);
  m_updateBodiesProgram->SetUniform1f("deltaTime", deltaTime);
  m_updateBodiesProgram->SetUniform1i("nBodies", m_nBodies);
  m_updateBodiesProgram->SetUniform2f("MousePos", mouse_pos.x, mouse_pos.y);
  m_updateBodiesProgram->Bind();

  glDispatchCompute(m_nBodies, 1, 1);
}

void Field::render() {
  JAGE::Renderer renderer;
  renderer.Clear();

  m_bodiesPos->Bind(2);
  m_renderProgram->SetUniform1i("nBodies", m_nBodies);
  m_renderProgram->SetUniform2f("WindowSize", m_window->getWidth(), m_window->getHeight());
  renderer.Draw(*m_triangles, *m_triangles_indicies, *m_renderProgram);

  glfwSwapBuffers(m_window->getWindow());
  glfwPollEvents();
}
