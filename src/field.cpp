#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <window.h>
#include <renderer.h>
#include <shader_storage_buffer.h>
#include "field.hpp"
#include "vertex_buffer.h"

Field::Field(unsigned int nOfBodies) {
  m_nBodies = nOfBodies;

  // Initialize bodies positions
  glm::vec2* initial_pos = new glm::vec2[nOfBodies];
  for(int i = 0; i < nOfBodies; i++)
    initial_pos[i] = glm::vec2(0.0f, 0.0f);
  m_bodiesPos = new JAGE::ShaderStorageBuffer(initial_pos, nOfBodies*sizeof(glm::vec2), GL_DYNAMIC_COPY);

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

  delete[] initial_pos;
}

Field::~Field() {
  delete m_bodiesPos;
  delete m_renderProgram;
  delete m_updateBodiesProgram;
  delete m_triangles;
  delete m_triangles_indicies;
}

void Field::update(float deltaTime) {
  m_bodiesPos->Bind(2);
  m_updateBodiesProgram->SetUniform1f("deltaTime", deltaTime);
  m_updateBodiesProgram->SetUniform1i("nBodies", m_nBodies);
  m_updateBodiesProgram->Bind();

  glDispatchCompute(m_nBodies, 1, 1);

  glm::vec2* data = (glm::vec2*)m_bodiesPos->GetData();
  for(int i = 0; i < m_nBodies; i++) {
    std::cout << data[i].x << " " << data[i].y << "\n";
  }
  delete[] data;
}

void Field::render(JAGE::Window* window) {
  JAGE::Renderer renderer;
  renderer.Clear();

  renderer.Draw(*m_triangles, *m_triangles_indicies, *m_renderProgram);

  glfwSwapBuffers(window->getWindow());
  glfwPollEvents();
}
