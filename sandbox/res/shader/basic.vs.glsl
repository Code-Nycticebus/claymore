#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_translation;
layout (location = 2) in vec4 a_color;

out vec4 v_color;

void main() {
  vec3 pos = a_pos + a_translation;
  gl_Position = vec4(pos.xyz, 1.0);
  v_color = a_color;
}
