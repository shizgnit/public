precision mediump float;
precision mediump int;

uniform mat4 u_Parameters;

flat varying uint v_Identifier;
flat varying uint v_Flags;

void main()
{
  float tmp = floor(float(v_Flags) / 2.0);
  float bit = mod(tmp, 2.0);

  if(bit != 1.0) {
    discard;
  }

  float unit = (1.0 / 255.0);
  float upper = unit * float(v_Identifier / uint(255));
  float lower = unit * float(v_Identifier % uint(255));

  gl_FragColor = vec4(u_Parameters[1][1], upper, lower, 1.0);
}