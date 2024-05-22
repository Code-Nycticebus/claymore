#version 330 core


out vec4 f_color;

in vec3 v_normal;
in vec3 v_frag_pos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

uniform vec3 u_light_pos;
uniform vec4 u_light_color;
uniform vec3 u_view_pos;

void main() {
  vec3 light_color = u_light_color.rgb;
  // ambient
  vec3 ambient = light_color * material.ambient;
  
  // diffuse 
  vec3 norm = normalize(v_normal);
  vec3 light_dir = normalize(u_light_pos - v_frag_pos);
  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = light_color * (diff * material.diffuse);
  
  // specular
  vec3 view_dir = normalize(u_view_pos - v_frag_pos);
  vec3 reflect_dir = reflect(-light_dir, norm);  
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess*128);
  vec3 specular = light_color * (spec * material.specular);  
      
  vec3 result = ambient + diffuse + specular;
  f_color = vec4(result, 1.0);

  // Debugging
  // f_color = vec4((v_normal + 1) / 2, 1.0);
}
