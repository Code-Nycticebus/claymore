#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 2) in vec2 a_uv;

out vec2 v_uv;

uniform mat4 u_vp;

void main() {
  gl_Position = u_vp * vec4(a_pos, 1.0);
  v_uv = a_uv;
}
