#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform uint u_Flags;

void main()
{
  if(u_Flags >= uint(0x01)) {
    gl_FragColor = vec4(vec3(0.0), 1.0);
  }
  else {
    gl_FragColor = vec4(vec3(1.0 - gl_FragCoord.z), 1.0);
  }
}
