#version 430

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform int nBodies;
uniform float deltaTime;

uniform vec2 MousePos;

layout(std430, binding = 2) buffer bodies {
  vec4 body[];
};

void main() {
  uint index = gl_WorkGroupID.x;

  body[index].zw += deltaTime * (0.7 / pow(distance(body[index].xy, MousePos), 2.0) ) * normalize(MousePos - body[index].xy);
  body[index].zw = clamp(body[index].zw, vec2(-10, -10), vec2(10, 10));
  body[index].xy += 0.1 * deltaTime * body[index].zw;
}
