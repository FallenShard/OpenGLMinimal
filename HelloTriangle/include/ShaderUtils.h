#pragma once

#include <OpenGL.h>

#include <iostream>
#include <fstream>
#include <string>

// Prototypes
GLint  createProgram(std::string vShader, std::string fShader);
GLint  createShader(GLenum type, std::string fileName);
void    loadFile(std::string fileName, std::string& out);
bool    hasCompileError(GLint shaderId);
bool    hasLinkError(GLint programId);

// Implementation
GLint createProgram(std::string vShader, std::string fShader)
{
    // Create a vertex shader, check function below
    GLint vShaderId = createShader(GL_VERTEX_SHADER, vShader);
    if (vShaderId == -1)
        return -1;

    // Create a fragment shader, check function below
    GLint fShaderId = createShader(GL_FRAGMENT_SHADER, fShader);
    if (fShaderId == -1)
        return -1;

    GLint tcShaderId = createShader(GL_TESS_CONTROL_SHADER, "default.tesc");
    GLint teShaderId = createShader(GL_TESS_EVALUATION_SHADER, "default.tese");
    
    // Create a program which will link the shaders
    // So, after creating shaders, we need something to link them into a single unit
    // That's where programs come around
    GLint progId = glCreateProgram();

    // This function associates programId with a shaderId provided
    glAttachShader(progId, vShaderId);
    glAttachShader(progId, fShaderId);
    glAttachShader(progId, tcShaderId);
    glAttachShader(progId, teShaderId);

    // Linking the program, will produce linked binary code for the GPU to use
    glLinkProgram(progId);

    // We can check for errors, but there won't be any in this example
    if (hasLinkError(progId))
        return -1;

    // We can delete shaders now, but they won't exactly be deleted, as long as the program is alive
    // that is until we call glDeleteProgram(progId), but we won't do that, we need the program to render
    glDeleteShader(vShaderId);
    glDeleteShader(fShaderId);

    // Return valid program
    return progId;
}

GLint createShader(GLenum type, std::string fileName)
{
    // Okay, so in order to create a shader, we need source, either in string or in text file, whatever
    // But first we need a handle to it, that's where glCreateShader(GLenum shaderType) comes in
    // with the first call above, we want to make a vertex shader
    GLint shaderId = glCreateShader(type);

    // Load source into a string, it doesn't have anything to do with OpenGL, basic I/O
    std::string shaderSrc = "";
    loadFile(fileName, shaderSrc);

    // This is required so the next function will work properly
    const GLchar* source = shaderSrc.c_str();

    // glShaderSource associates shaderid with the source we told it
    // first param is shaderId, second is lines of code in source (1, we put everything in one long string)
    // source (is basically a char matrix, but a 1 x length matrix)
    // length, in case our source is not null terminated, but it will be, as std::string takes care of that
    glShaderSource(shaderId, 1, &source, nullptr);

    // Compile the shader, after this it is ready to be linked into a program, and we only need its handle
    glCompileShader(shaderId);

    // We can check for errors if something went wrong, unimportant for now as this example will compile properly
    if (hasCompileError(shaderId))
        return -1;

    return shaderId;
}

void loadFile(std::string fileName, std::string& outData)
{
    std::ifstream inputFile(fileName);

    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            outData += line + '\n';
        }
        inputFile.close();
    }
}

bool hasCompileError(GLint id)
{
    // Result from the following gl function will be written in compileStatus
    GLint compileStatus = 0;

    // Assume that there was no error
    bool hasError = 0;

    // Get compilation status of a shader
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

    // If GL_FALSE was written in compileStatus, there was an error with the shader
    if (compileStatus == GL_FALSE)
    {
        hasError = true;
        int infoLogLength = 0;

        // Log the additional info
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];
        GLsizei charsWritten = 0;
        glGetShaderInfoLog(id, infoLogLength, &charsWritten, buffer);

        std::cout << buffer << '\n';

        delete[] buffer;
    }

    return hasError;
}

bool hasLinkError(GLint id)
{
    // Used to grab actual error status
    GLint linkStatus = 0;

    // Assume that there was no error
    bool hasError = 0;

    // Get linking status of a program
    glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);

    // If GL_FALSE was written in linkStatus, there was an error with linking
    if (linkStatus == GL_FALSE)
    {
        hasError = true;
        int infoLogLength = 0;

        // Log the additional info
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];
        GLsizei charsWritten = 0;
        glGetProgramInfoLog(id, infoLogLength, &charsWritten, buffer);

        std::cout << buffer << '\n';

        delete[] buffer;
    }

    return hasError;
}