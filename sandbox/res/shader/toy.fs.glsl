#version 430 core

layout(location = 0) out vec4 frag_color;

uniform float u_time;
uniform int u_frame;
uniform vec2 u_resolution;

uniform float u_deltatime;
uniform float u_fps;

void main() {
	vec2 st = gl_FragCoord.xy/u_resolution;
	frag_color = vec4(st.x,st.y,0.0,1.0);
}
