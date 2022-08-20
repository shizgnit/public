precision mediump float;
precision mediump int;

uniform sampler2D u_SurfaceTextureUnit;
uniform sampler2D u_ShadowTextureUnit;
uniform sampler2D u_NormalTextureUnit;

uniform vec4 u_AmbientLightPosition;
uniform vec4 u_AmbientLightColor;

varying vec2 v_Texture;
varying vec4 v_Vertex;
varying vec4 v_Normal;
varying vec4 v_Lighting;

void main()
{
    vec3 Projection = v_Lighting.xyz / v_Lighting.w;
    Projection = Projection * 0.5 + 0.5;

    gl_FragColor = vec4(vec3(Projection.z), 1.0);
}