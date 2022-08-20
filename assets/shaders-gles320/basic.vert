#version 320 es

precision mediump float;
precision mediump int;

layout(location = 0) in mat4 a_ModelMatrix;

layout(location = 6) in vec4 a_Vertex;
layout(location = 7) in vec4 a_Texture;
layout(location = 8) in vec4 a_Normal;

layout(location = 9) uniform mat4 u_ProjectionMatrix;
layout(location = 10) uniform mat4 u_ViewMatrix;
layout(location = 11) uniform mat4 u_ModelMatrix;
layout(location = 12) uniform mat4 u_LightingMatrix;

layout(location = 13) uniform vec4 u_Clipping;

layout(location = 14) uniform vec4 u_AmbientLightPosition;
layout(location = 15) uniform vec4 u_AmbientLightColor;

layout(location = 16) uniform uint u_Flags;
layout(location = 17) uniform mat4 u_Parameters;

out vec4 v_Vertex;
out vec2 v_Texture;
out vec4 v_Normal;
out vec4 v_Lighting;
out vec4 v_Clipping;

out uint v_Flags;

void main()
{
  mat4 MVP = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix;

  v_Texture = a_Texture.xy;
  v_Vertex = MVP * a_Vertex;
  v_Normal = normalize(MVP * a_Normal);
  
  gl_Position = v_Vertex;

  vec4 model = u_ModelMatrix * a_Vertex;

  float top = dot( model, vec4( 0.0, -1.0, 0.0, u_Clipping.x ));
  float bottom = dot( model, vec4( 0.0, 1.0, 0.0, u_Clipping.y ));
  float left = dot( model, vec4( 1.0, 0.0, 0.0, u_Clipping.z ));
  float right = dot( model, vec4( -1.0, 0.0, 0.0, u_Clipping.w ));

  v_Clipping = vec4(top, bottom, left, right);
}

// ERROR: 0:37: 'assign' :  cannot convert from 'attribute 4-component vector of mediump float' to 'varying 2-component vector of mediump float'
