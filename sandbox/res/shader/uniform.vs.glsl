#version 330 core

layout (location = 0) in vec3 a_pos;

uniform mat4 u_mvp;

void main() {
  gl_Position = u_mvp * vec4(a_pos.xyz, 1.0);
}
