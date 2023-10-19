#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in mat4 transform;

uniform mat4 u_vp;
uniform mat4 u_model;

out vec4 v_color;
out vec3 v_normal;
out vec3 v_frag_pos;

mat3 extractRotationMatrix(mat4 modelMatrix) {
    return mat3(modelMatrix[0].xyz, modelMatrix[1].xyz, modelMatrix[2].xyz);
}

void main() {
  mat4 model_transform = u_model * transform;
  vec4 model_space = model_transform * vec4(a_pos, 1.0);
  gl_Position = u_vp * model_space;
  v_color = a_color;
  v_frag_pos = model_space.xyz;

  mat3 normal_mat = extractRotationMatrix(model_transform);
  v_normal = vec3(normal_mat * a_normal);
}
