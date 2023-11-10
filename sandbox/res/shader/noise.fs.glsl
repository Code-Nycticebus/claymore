#version 330 core

out vec4 f_color;

in vec2 v_uv;

uniform sampler2D u_texture;

void main() {
  vec4 texture = texture(u_texture, v_uv);
  vec4 color = vec4(texture.r, texture.r, texture.r, 1);
  f_color = color;
}
