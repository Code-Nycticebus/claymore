#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;
layout (location = 2) in mat4 transform;

uniform mat4 u_mvp;

out vec4 v_color;

void main() {
  gl_Position = u_mvp * (transform * vec4(a_pos.xyz, 1.0));
  v_color = a_color;
}
