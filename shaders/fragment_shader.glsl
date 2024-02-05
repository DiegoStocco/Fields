#version 430

out vec3 color;

uniform int nBodies;
layout(std430, binding = 2) buffer bodies {
  vec2 pos[];
};

void main() {
  color = vec3(1, 1, 1);
  float max_d = -1, min_d = 99999999, d;
  for(int i = 0; i < nBodies; i++) {
    d = distance(gl_FragCoord.xy, pos[i]);
    if(d > max_d) {max_d = d;}
    if(d < min_d) {min_d = d;}
  }

  color *= min_d/2;
  color.xy = gl_FragCoord.xy;
}
