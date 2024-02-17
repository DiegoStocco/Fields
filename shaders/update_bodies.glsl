#version 430

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

uniform int nBodies;
uniform float deltaTime;

layout(std430, binding = 2) buffer bodies {
  vec4 body[];
};

void main() {
  body[gl_WorkGroupID.x].zw += deltaTime*vec2(cos(radians(360) * body[gl_WorkGroupID.x].y),
                                          -sin(radians(360) * body[gl_WorkGroupID.x].x));
  body[gl_WorkGroupID.x].xy += deltaTime * body[gl_WorkGroupID.x].zw;
}
