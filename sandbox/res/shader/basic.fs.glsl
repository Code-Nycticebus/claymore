#version 330 core

out vec4 f_color;

in vec4 v_color;

uniform vec2 u_mouse_pos;
uniform float u_distance;

void main() {
    // Calculate the distance from the current pixel to the mouse position
    vec2 pixelPos = gl_FragCoord.xy;
    float distance = distance(u_mouse_pos.xy, pixelPos);  

    // Calculate the alpha based on the distance
    float alpha = smoothstep(distance, 0.0, u_distance);

    // Mix the original color (v_color) with a fully transparent color (vec4(0.0)) based on the alpha
    f_color = mix(v_color, vec4(0.0), alpha);
}
