#version 330 core

out vec4 f_color;

in vec4 v_color;


uniform vec4 u_color;

void main() {
  f_color = v_color * u_color;
}
