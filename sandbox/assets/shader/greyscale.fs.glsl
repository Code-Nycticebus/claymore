#version 330 core

out vec4 f_color;
in vec2 v_uv;

uniform sampler2D u_texture;

void main()
{
    vec4 texture_color = texture(u_texture, v_uv);
    float grey = (texture_color.r + texture_color.g + texture_color.b) / 3.0;
    f_color = vec4(grey, grey, grey, texture_color.a);
}

