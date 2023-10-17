#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec4 i_color;
layout (location = 3) in mat4 transform;

uniform mat4 u_vp;
uniform mat4 u_model;

out vec4 v_color;
out vec3 v_normal;
out vec3 v_frag_pos;

void main() {
  vec4 model_space = u_model * transform * vec4(a_pos, 1.0);
  gl_Position = u_vp * model_space;
  v_color = i_color;
  v_frag_pos = model_space.xyz;
  v_normal = a_normal;
}
