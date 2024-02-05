#version 430

layout(location = 0) in vec2 vertex_pos;

void main() {
  gl_Position.xy = vertex_pos;
  gl_Position.zw = vec2(0.0, 1.0);
}
