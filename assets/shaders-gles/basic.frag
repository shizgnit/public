#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D u_SurfaceTextureUnit;
uniform sampler2D u_ShadowTextureUnit;
uniform sampler2D u_NormalTextureUnit;

uniform vec4 u_Clipping;
uniform vec4 u_AmbientLightPosition;
uniform vec4 u_AmbientLightColor;

varying vec4 v_Vertex;
varying vec2 v_Texture;
varying vec4 v_Normal;
varying vec4 v_Clipping;

// out vec4 diffuseColor;

void main()
{
  if ( v_Clipping.x < 0.0 )
    discard;

  if ( v_Clipping.y < 0.0 )
    discard;

  if ( v_Clipping.z < 0.0 )
    discard;

  if ( v_Clipping.w < 0.0 )
    discard;

  gl_FragColor = texture2D(u_SurfaceTextureUnit, v_Texture.xy);
}
