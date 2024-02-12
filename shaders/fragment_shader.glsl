#version 430

out vec3 color;

uniform vec2 WindowSize;

uniform int nBodies;
layout(std430, binding = 2) buffer bodies {
  vec2 pos[];
};

void main() {
  vec2 pixel_pos = 2*vec2(gl_FragCoord.x/WindowSize.x, gl_FragCoord.y/WindowSize.y) - vec2(1,1);
 
  float max_d = -1, min_d = 99999999, d;
  for(int i = 0; i < nBodies; i++) {
    d = distance(pixel_pos, pos[i]);
    if(d > max_d) {max_d = d;}
    if(d < min_d) {min_d = d;}
  }

  color.x = 1-pow(min_d/(2*sqrt(2)), 0.1);
  color.y = pow(min_d/(2*sqrt(2)), 0.5);
  color.z = 1-pow(min_d/(2*sqrt(2)), 0.3);
}
