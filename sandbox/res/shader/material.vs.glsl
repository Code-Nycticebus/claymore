#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normals;
layout (location = 2) in mat4 transforms;

uniform mat4 u_vp;

out vec3 v_frag_pos;
out vec3 v_normal;

mat3 extractRotationMatrix(mat4 modelMatrix) {
    return mat3(modelMatrix[0].xyz, modelMatrix[1].xyz, modelMatrix[2].xyz);
}

void main() {
  vec4 model_space = transforms * vec4(a_pos, 1.0);
  gl_Position = u_vp * model_space;
    
  v_frag_pos = model_space.xyz;

  mat3 normal_mat = extractRotationMatrix(transforms);
  v_normal = vec3(normal_mat * a_normals);
}
