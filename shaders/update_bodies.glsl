#version 430

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform int nBodies;
uniform float deltaTime;

layout(std430, binding = 2) buffer bodies {
  vec2 pos[];
};

void main() {
  pos[gl_WorkGroupID.x] += deltaTime/20;
}
