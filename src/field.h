#pragma once
#include <window.h>

class Field {
  public:
    void update(float deltaTime);
    void render(JAGE::Window* window);
};
