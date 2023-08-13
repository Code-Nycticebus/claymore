#version 120

varying vec2 v_uv;
uniform sampler2D u_texture;

void main() {
  gl_FragColor = vec4(texture2D(u_texture, v_uv).r);
}
