// This directive specified GLSL version used, its 3.3 in here
// core means core profile, with no backward compatibility features
// first number is major version, next two numbers minor
// if we wanted 4.1 it would be 410
#version 330 core

// This is the variable that came from vertexShader, interpolated by rasterizer
// Smooth means smooth interpolation, change to "flat" in here and in vertex shader
// and see what happens
// in means that it is an input variable, coming from one of previous vertex stages
// such as vertex shader, vec4 is well, array of 4 floats and fColor has to match the 
// name specified in vertexShader
smooth in vec4 fColor;

// Fragment shader's only purpose is to provide color to a pixel, as it works per-pixel (in parallel)
// There's no other way for that color to go anywhere, so we specif
out vec4 finalColor;

void main()
{
    // That's about it, we simply pass interpolated color to final color for the pixel
    finalColor = fColor;
}