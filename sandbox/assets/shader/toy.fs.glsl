

vec3 palette(float t) {
  vec3 a = vec3(0.3, 0.2, 0.5);
  vec3 b = vec3(0.2, 0.9, 0.7);
  vec3 c = vec3(0.2, 0.23, 1.0);
  vec3 d = vec3(0.3112, 0.213, 0.642);

  return a + b * cos(6.28318 * (c * t + d));
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
  vec2 uv = fragCoord.xy / u_resolution * 2 - 1;
  vec2 uv0 = uv;
  // vec2 uv0 = uv * u_resolution.x / u_resolution.y;
  vec3 finalColor = vec3(0);

  for (float i = 0.0; i < 2.0; i++) {
    uv = fract(uv * 1.9) - 0.5;
    float d = length(uv) * exp(-length(uv0));

    vec3 col = palette(length(uv0) + i*.8 + u_time*.4);
    
    const float freq = 8.;
    d = cos(d*freq+u_time*.4)/freq;
    d = abs(d);

    d = pow(0.01 / d, 1.2);

    finalColor += col *= d;
  }

  fragColor = vec4(finalColor, 1);
}
