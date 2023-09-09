#version 330 core

out vec4 f_color;
in vec2 v_uv;

uniform sampler2D u_texture;

void main()
{
    vec4 text_color = texture(u_texture, v_uv);
    float grey = (text_color.r + text_color.g + text_color.b) / 3.0;
    f_color = vec4(grey, grey, grey, text_color.a);

}

