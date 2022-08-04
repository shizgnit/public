precision mediump float;
precision mediump int;

in mat4 a_ModelMatrix;
in vec4 a_Vertex;
in vec4 a_Texture;
in vec4 a_Normal;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix; 
uniform mat4 u_LightingMatrix;
uniform vec4 u_Clipping;

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
