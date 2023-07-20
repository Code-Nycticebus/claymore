#version 330 core

out vec4 f_color;

in vec4 v_color;

uniform vec2 u_mouse_pos;
uniform float u_distance;

void main() {
    float distance = distance(u_mouse_pos.xy, gl_FragCoord.xy);  

    if (distance < u_distance) {
      f_color = v_color;
    } else {
      f_color = vec4(0.0);
    }
}
