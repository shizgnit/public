#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform mat4 u_Parameters;

flat varying uint v_Identifier;

void main()
{ 
  float unit = (1.0 / 255.0);
  float upper = unit * float(v_Identifier / uint(255));
  float lower = unit * float(v_Identifier % uint(255));
  
  gl_FragColor = vec4(u_Parameters[1][1], upper, lower, 1.0);
}