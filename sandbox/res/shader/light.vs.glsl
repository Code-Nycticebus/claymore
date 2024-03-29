#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normals;

uniform mat4 u_vp;

out vec4 v_color;
out vec3 v_normal;
out vec3 v_frag_pos;



void main() {
  vec4 model_space = vec4(a_pos, 1.0);
  gl_Position = u_vp * model_space;
  v_color = vec4(0.2, 0.2, 0.2, 1);
  v_frag_pos = model_space.xyz;
  v_normal = a_normals;
}
