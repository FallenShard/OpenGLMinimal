#version 440 core

layout(quads, equal_spacing, cw) in;

in TCSData
{
    vec4 color;
} tcsData[];

//out TESData
//{
//    vec4 color;
//} tesData[];

out vec4 tesColor;

void main()
{
    tesColor = mix(tcsData[0].color, tcsData[2].color, gl_TessCoord.x);
    //tesColor += mix(tcsData)

    gl_Position = gl_in[0].gl_Position;
    gl_Position.xy += gl_TessCoord.xy;
}