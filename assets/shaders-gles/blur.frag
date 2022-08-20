precision mediump float;
precision mediump int;

uniform sampler2D u_SurfaceTextureUnit;
uniform sampler2D u_NormalTextureUnit;
uniform sampler2D u_ShadowTextureUnit;
uniform sampler2D u_DepthTextureUnit;
uniform sampler2D u_BlurTextureUnit;

uniform vec4 u_Clipping;
uniform vec4 u_AmbientLightPosition;
uniform vec4 u_AmbientLightColor;
uniform uint u_Flags;
uniform mat4 u_Parameters;

varying vec4 v_Vertex;
varying vec2 v_Texture;
varying vec4 v_Normal;

void main()
{
  vec4 color = texture(u_SurfaceTextureUnit, v_Texture.xy);
  float count = 0.0;

  int seed = int(u_Parameters[0][0]);

  vec2 texelSize = vec2(1.0) / vec2(textureSize(u_SurfaceTextureUnit, 0));
  for(int x = -seed; x <= seed; ++x) {
    for(int y = -seed; y <= seed; ++y) {
      color.rgb += texture(u_SurfaceTextureUnit, v_Texture.xy + vec2(x, y) * texelSize).rgb;
      count += 1.0;
    }
  }

  color.rgb /= count;

  gl_FragColor = color;
}
