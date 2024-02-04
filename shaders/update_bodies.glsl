#version 430

uniform int nBodies;
uniform float deltaTime;

layout(std430, binding = 2) buffer bodies {
  vec2 pos[];
};

void main() {
  pos[gl_WorkGroupID.x] += deltaTime;
}
