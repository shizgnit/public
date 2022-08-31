#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[position]];
    float3 normal;
    half3 color;
    float2 texcoord;
};

struct VertexData
{
    float4 padding1;
    float4 position;
    float4 padding2;
    float4 texcoord;
    float4 padding3;
    float4 normal;
};

struct InstanceData
{
    float4 padding;
    float4x4 modelTransform;
};

struct CameraData
{
    float4x4 perspectiveTransform;
    float4x4 viewTransform;
    float4x4 modelTransform;
    float4x4 lightingTransform;
};

v2f vertex vertexMain( device const VertexData* vertexData [[buffer(0)]],
                        device const CameraData& cameraData [[buffer(1)]],
                        device const InstanceData* instanceData [[buffer(2)]],
                        uint vertexId [[vertex_id]],
                        uint instanceId [[instance_id]] )
{
    v2f out;

    const device VertexData& vd = vertexData[ vertexId ];
    float4 pos = vd.position;
    pos = instanceData[ instanceId ].modelTransform * pos;
    pos = cameraData.perspectiveTransform * cameraData.viewTransform * pos;
    out.position = pos;

    float4 normal = instanceData[ instanceId ].modelTransform * vd.normal;
    normal = cameraData.viewTransform * normal;
    //out.normal = normal;
    out.normal = float3(1.0, 0.0, 0.0);

    out.texcoord = vd.texcoord.xy;

    //out.color = half3( instanceData[ instanceId ].instanceColor.rgb );
    return out;
}

half4 fragment fragmentMain( v2f in [[stage_in]], texture2d< half, access::sample > tex [[texture(0)]] )
{
    half3 color = half3( 0.5, 0.5, 0.5 );

    constexpr sampler s( address::repeat, filter::linear );
    half3 texel = tex.sample( s, in.texcoord.xy ).rgb;

    // assume light coming from (front-top-right)
    float3 l = normalize(float3( 1.0, 1.0, 0.8 ));
    float3 n = normalize( in.normal.xyz );

    half ndotl = half( saturate( dot( n, l ) ) );

    half3 illum = texel; //(color * texel * 0.1) + (color * texel * ndotl);
    return half4( illum, 1.0 );
}

