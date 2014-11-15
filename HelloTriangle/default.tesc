#version 440 core

layout(vertices = 4) out;

in VertexData
{
    vec4 color;
} vData[];

out TCSData
{
    vec4 color;
} tcsData[];

void main()
{
    tcsData[gl_InvocationID].color = vData[gl_InvocationID].color;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelOuter[0] = 4.0;
    gl_TessLevelOuter[1] = 4.0;
    gl_TessLevelOuter[2] = 4.0;
    gl_TessLevelOuter[3] = 4.0;
    gl_TessLevelInner[0] = 4.0;
    gl_TessLevelInner[1] = 4.0;
}