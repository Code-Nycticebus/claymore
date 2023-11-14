#version 330 core

out vec4 f_color;

in vec2 v_uv;

uniform float u_time;

vec2 random_gradient(vec2 p) {
  p = p+0.01;
  float x = dot(p, vec2(123.4, 234.5));
  float y = dot(p, vec2(234.5, 345.6));

  vec2 gradient = vec2(x,y);
  gradient = gradient * 43758.5453;


  gradient = sin(gradient + u_time);
  return gradient;
}

float perlin_noise(vec2 uv) {
  // Grid cells
  uv = uv * 4.0;
  vec2 grid_id = floor(uv);
  vec2 grid_uv = fract(uv);


  vec2 bl = grid_id + vec2(0.0, 0.0);
  vec2 br = grid_id + vec2(1.0, 0.0);
  vec2 tl = grid_id + vec2(0.0, 1.0);
  vec2 tr = grid_id + vec2(1.0, 1.0);


  vec2 grad_bl = random_gradient(bl);
  vec2 grad_br = random_gradient(br);
  vec2 grad_tl = random_gradient(tl);
  vec2 grad_tr = random_gradient(tr);


  vec2 distance_px_to_bl = grid_uv - vec2(0.0, 0.0);
  vec2 distance_px_to_br = grid_uv - vec2(1.0, 0.0);
  vec2 distance_px_to_tl = grid_uv - vec2(0.0, 1.0);
  vec2 distance_px_to_tr = grid_uv - vec2(1.0, 1.0);


  float dot_bl = dot(grad_bl, distance_px_to_bl);
  float dot_br = dot(grad_br, distance_px_to_br);
  float dot_tl = dot(grad_tl, distance_px_to_tl);
  float dot_tr = dot(grad_tr, distance_px_to_tr);

  grid_uv = smoothstep(0.0, 1.0, grid_uv);

  float b = mix(dot_bl, dot_br, grid_uv.x);
  float t = mix(dot_tl, dot_tr, grid_uv.x);
  float perlin = mix(b, t, grid_uv.y);

  return perlin;
}

float fbm_perlin_noise(vec2 uv) {
  float fbm_noise = 0.0;
  float amplitude = 1.0;
  const float octaves = 7.0;

  for (float i = 0.0; i < octaves; i++) {
    fbm_noise = fbm_noise + perlin_noise(uv) * amplitude;
    amplitude * amplitude * 0.5;
    uv = uv * 2.0;
  }

  return fbm_noise;
}

void main() {
  vec3 color; 
  vec2 resolution = vec2(500, 500);

  vec2 uv = gl_FragCoord.xy / resolution;

  float perlin = fbm_perlin_noise(uv);
  color = vec3(perlin);

  // float billow = abs(perlin);
  // color = vec3(billow);

  // float ridged_noise = 1.0 - abs(perlin);
  // ridged_noise = ridged_noise * ridged_noise;
  // color = vec3(ridged_noise);

  f_color = vec4(color, 1.0);
}
