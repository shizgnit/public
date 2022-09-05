#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D u_SurfaceTextureUnit;
uniform sampler2D u_ShadowTextureUnit;
uniform sampler2D u_NormalTextureUnit;

uniform vec4 u_AmbientLightPosition;
uniform vec4 u_AmbientLightColor;
uniform uint u_Flags;
uniform mat4 u_Parameters;

varying vec2 v_Texture;
varying vec4 v_Vertex;
varying vec4 v_Normal;
varying vec4 v_Lighting;

flat varying uint v_Identifier;
flat varying uint v_Flags;

void main()
{
  vec4 L = normalize(u_AmbientLightPosition - v_Vertex);
  vec4 E = normalize(-v_Vertex);
  vec4 R = normalize(-reflect(L,v_Normal));
  
  float d = max(dot(v_Normal, L), 0.0);
  float c = 0.4;
  
  if (d > 0.95)
  c = 1.2;
  else if (d > 0.5)
    c = 1.0;
  else if (d > 0.25)
    c = 0.5;
  
  vec4 Light = vec4(c, c, c, 1.0);

  float Bit = mod(float(v_Flags), 2.0);
  if(Bit == 1.0) {
    Light = vec4(2.0, 2.0, 2.0, 1.0);
  }

  gl_FragColor = texture(u_SurfaceTextureUnit, v_Texture) * vec4(texture(u_SurfaceTextureUnit, v_Texture).a) * Light;
}