#pragma once
#include "index_buffer.h"
#include "vertex_array.h"
#include <window.h>
#include <shader.h>
#include <shader_storage_buffer.h>

class Field {
  public:
    Field(unsigned int nOfBodies);
    ~Field();

    void update(float deltaTime);
    void render(JAGE::Window* window);

  private:
    JAGE::Shader* m_renderProgram;
    JAGE::Shader* m_updateBodiesProgram;

    JAGE::ShaderStorageBuffer* m_bodiesPos;
    unsigned int m_nBodies;

    JAGE::VertexArray* m_triangles;
    JAGE::IndexBuffer* m_triangles_indicies;
};
