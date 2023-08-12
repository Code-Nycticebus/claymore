#version 330 core

out vec4 f_color;

in vec2 v_uv;

uniform sampler2D u_texture;

void main() {
  vec4 texture_color = texture(u_texture, v_uv);
  f_color = texture_color;
}
