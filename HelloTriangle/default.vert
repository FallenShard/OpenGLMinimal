#version 330 core
// Okay, we sent two attributes down the pipeline, positions as 3 floats
// remember the x,y,z and color as rgba (4 floats) per vertex
// Vertex shaders as noted, work PER VERTEX
// we have 3 vertices, this vertex shader will run 3 times in parallel for each vertex
in vec3 position;
in vec4 color;

// This is the color for fragment shader, vertex shader's job is to determine POSITION for each vertex
// so we pass the color, it will be interpolated smoothly on the way (as specified by 'smooth' keyword)
// out means it will be handled in next shader stage (fragment shader)
// vec4 is array of 4 floats, fColor is just a name we picked (it has to be same on the other side though)
smooth out vec4 fColor;

void main()
{
    // So we pass that color we received from the program, in that vertex buffer, as described there
    fColor = color;

    // Let's determine that position

    // gl_Position is PREDEFINED Glsl variable
    // each vertex has one
    // we simply pass the vertex and add one more dimension (1.f, will be explained later with 3D)
    gl_Position = vec4(position, 1.f);
}