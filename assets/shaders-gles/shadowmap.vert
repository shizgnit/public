precision mediump float;
precision mediump int;

attribute mat4 a_ModelMatrix;
attribute vec4 a_Vertex;
attribute vec4 a_Texture;
attribute vec4 a_Normal;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_LightingMatrix;
uniform vec4 u_Clipping;
uniform vec4 u_AmbientLightPosition;
uniform vec4 u_AmbientLightColor;
uniform uint u_Flags;
uniform mat4 u_Parameters;

varying vec4 v_Vertex;
varying vec2 v_Texture;
varying vec4 v_Normal;
varying vec4 v_Lighting;
varying vec4 v_Clipping;
varying uint v_Flags;

void main()
{
  mat4 MVP = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix;
  if(gl_InstanceID >= 1) {
    MVP = u_ProjectionMatrix * u_ViewMatrix * a_ModelMatrix;
  }

  v_Vertex = MVP * a_Vertex;
  v_Normal = normalize(MVP * a_Normal);

  gl_Position = v_Vertex;

  if(gl_InstanceID >= 1) {
    v_Lighting = (u_LightingMatrix * a_ModelMatrix) * a_Vertex;
  }
  else {
    v_Lighting = (u_LightingMatrix * u_ModelMatrix) * a_Vertex;
  }
}
