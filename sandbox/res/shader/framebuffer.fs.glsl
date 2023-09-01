#version 330 core

out vec4 f_color;
in vec2 v_uv;
uniform sampler2D screen_texture;

void main()
{
    f_color = texture(screen_texture, v_uv);
}

