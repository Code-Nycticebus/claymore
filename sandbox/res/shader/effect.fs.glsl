#version 330 core

out vec4 f_color;
in vec2 v_uv;

uniform sampler2D u_texture;
uniform float u_gamma = 2.0;

void main()
{
    vec4 fragment = texture(u_texture, v_uv);
    f_color.rgb = pow(fragment.rgb, vec3(1.0 / u_gamma));
    f_color.a = 1.0;
}

