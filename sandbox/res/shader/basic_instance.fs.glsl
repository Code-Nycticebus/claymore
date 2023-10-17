#version 330 core

out vec4 f_color;

in vec4 v_color;
in vec3 v_normal;
in vec3 v_frag_pos;


void main() {
  f_color = v_color;
}
