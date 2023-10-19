#version 330 core

out vec4 f_color;

in vec4 v_color;
// in vec3 v_normal;
// in vec3 v_frag_pos;

// uniform vec3 u_light_pos;
// uniform vec3 u_light_color = vec3(1);
// uniform vec3 u_view_pos;

void main() {
  // vec3 norm = normalize(v_normal);
  // vec3 light_dir = normalize(u_light_pos - v_frag_pos);
  // vec3 light_color = u_light_color;

  // float specular_strenght = 0.5;
  // vec3 view_dir = normalize(u_view_pos - v_frag_pos);
  // vec3 reflect_dir = reflect(-light_dir, norm);

  // const float shininess = 256;
  // float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
  // vec3 specular = specular_strenght * spec * light_color;

  // float ambient_strenght = 0.1;
  // vec3 ambient = ambient_strenght * light_color;
  // 
  // float diff = max(dot(norm, light_dir), 0.0);
  // vec3 diffuse = diff * light_color;

  // vec3 color = (ambient + diffuse + specular) * v_color.rgb;
  // f_color = vec4(color, v_color.a);

  // Debugging
  // f_color = vec4((v_normal + 1) / 2, 1.0);


  f_color = v_color;
}
