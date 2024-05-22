

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
  vec2 uv = fragCoord.xy / u_resolution * 2 - 1;
  uv.x *= u_resolution.x / u_resolution.y;

  float d = length(uv);
  
  d -= 0.5;
  d = abs(d);

  fragColor = vec4(d, d, d, 1);
}
