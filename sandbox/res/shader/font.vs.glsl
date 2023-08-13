#version 120

attribute vec3 a_pos;
attribute vec2 a_uv;

varying vec2 v_uv;

uniform mat4 u_mvp;

void main() {
  gl_Position = u_mvp * vec4(a_pos.xyz, 1.0);
  v_uv = a_uv;
}
