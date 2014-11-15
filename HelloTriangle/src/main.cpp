#include <OpenGL.h>

#include <GL/freeglut.h>

#include "ShaderUtils.h"

// Handles for various OpenGL resources, described below
GLuint vao;
GLuint vbo, ibo;
GLint program;

GLuint noOfVertices = 4;

GLfloat vertices[] = 
{
  //  X      Y      Z
    -0.5f, -0.5f, -1.f, // Vertex 0
     0.5f, -0.5f, -1.f, // Vertex 1
     0.5f,  0.5f, -1.f, // Vertex 2
    -0.5f,  0.5f, -1.f, // Vertex 3 

  //  R    G    B    A
     1.f, 0.f, 1.f, 1.f, // Color for vertex 0
     0.f, 1.f, 0.f, 1.f, // Color for vertex 1
     0.f, 1.f, 1.f, 1.f, // Color for vertex 2
     1.f, 1.f, 1.f, 1.f
};

GLushort faces[] = 
{
    //0, 2, 3, // Only one face, we're drawing just one triangle
    0, 1, 2, 3
};

void init()
{
    // Please check file ShaderUtils.h
    program = createProgram("default.vert", "default.frag");

    // This function returns the position of position attribute in vertex shader from a valid shader program
    GLint positionLoc = glGetAttribLocation(program, "position");

    // This function returns the position of color attribute in vertex shader from a valid shader program
    GLint colorLoc = glGetAttribLocation(program, "color");

    // Generate our Vertex Array Object, VAOs encapsulate all the rendering information, such as which buffers to use and how to draw stuff
    glGenVertexArrays(1, &vao);

    // Generate a vertex buffer handle to store information about vertices (vertices[] array at the top)
    glGenBuffers(1, &vbo);

    // Bind the vbo to GL_ARRAY_BUFFER target so that it becomes the active buffer that receives commands (on that target)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Use the active buffer at target GL_ARRAY_BUFFER (in our case vbo), and populate it with data
    // 2nd argument is the size in bytes, third is pointer to data array and last is hint usage (not important for now)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Unbind vbo as GL_ARRAY_BUFFER, we're done using it for now
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Generate another buffer to use, this time index buffer for faces
    glGenBuffers(1, &ibo);
    
    // Index buffers are bound to GL_ELEMENT_ARRAY_BUFFER target, unlike vertex buffers (which are bound to GL_ARRAY_BUFFER)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Same as above, we tell OpenGL the size of the array, along with array pointer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
    
    // Unbind the array from this target, we're done "modifying" it (using it) for now
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Time to bind that VAO, it should contain all information about vertices for our triangle
    // VAOs have no target, there's just one type of VertexArray, the vertex array type itself, unlike buffers, of which there are multiple types
    glBindVertexArray(vao);
    
    // Bind the vertex array first, we'll be using some information for it
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // This function pulls data from currently bound GL_ARRAY_BUFFER, and tells the current VAO (bound vao above)
    // where to find data and what's the format of the buffer
    // We wrote our vertex data (vertices[] above) one attribute after another, that is, we specified position first and then color for each vertex
    // So we tell OpenGL to connect positionLoc (from the shader), to currently bound buffer's offset of 0 (last argument)
    // Other arguments to this function are data per vertex (3, we have xyz), type (GLfloat), and normalization(GL_FALSE, not important here)
    // The other zero (fifth argument), is stride, that is what's the offset in between positions (it's 0, they are tighly packed, one after another)
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
    // This function tells OpenGL to activate the positionLoc on currently bound VAO (vao, in our case)
    // It tells OpenGL to use that vertex attribute to feed into shader pipeline
    glEnableVertexAttribArray(positionLoc);

    // Color, pretty much the same as above, except that it has four components (rgba, so 4), and it has an offset (it's not at the start of the buffer)
    // It's offset in bytes is calculated below, and it also has it's own vertex array location (colorLoc)
    GLsizei colorOffset = noOfVertices * 3 * sizeof(GLfloat);
    glVertexAttribPointer(colorLoc,    4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)colorOffset);
    glEnableVertexAttribArray(colorLoc);
    // Function glVertexAttribPointer associates stuff found in active GL_ARRAY_BUFFER at the time of calling

    // Bind the ibo again, and this will automatically associate ibo with active VAO (in our case vao), just this call, nothing else
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    // At this point, we associated active GL_ARRAY_BUFFER (vbo) with our vao, and we associated active GL_ELEMENT_ARRAY_BUFFER(ibo) with our vao
    // That means we can "unbind" the vao, it has all the information it needs for rendering our triangle
    glBindVertexArray(0);

    // This simply sets the color used to clear the screen to what we want (dark gray here)
    // It DOESN'T clear the screen, glCLear(GL_COLOR_BUFFER_BIT) does that
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    
}

void display()
{
    // Clear the screen with specified color above
    glClear(GL_COLOR_BUFFER_BIT);			// Clear Color Buffers

    // Use our valid program for rendering
    glUseProgram(program);

    // Bind our vao, it holds all vertex layout data
    glBindVertexArray(vao);

    // Whichever we use from below will work
    //glDrawArrays(GL_TRIANGLES, 0, 4);

    // This call renders while using GL_ELEMENT_ARRAY_BUFFER we bound to vao above, as its index buffer
    // We need, type of primitve (GL_TRIANGLES here, it could be points, lines whatever)
    // How many face values we have total (3)
    // Type of index data (GLushort), as glenum that is GL_UNSIGNED_SHORT
    // And finally, the offset into our faces[] array we assigned to ibo (0, we want all that data, one face)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_SHORT, 0);
    glDrawArrays(GL_PATCHES, 0, 4);

    // Unbind vao, now, as you can see, to render multiple different objects, you bind vaos in a loop in successio, that's how easy it is
    glBindVertexArray(0);

    // Unbind program, maybe we want to use another one for a next object etc.
    glUseProgram(0);

    // Swap back and front buffer, not important to understand this right now
    glutSwapBuffers();
}

void cleanUp()
{
    // Delete what we allocated
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
}


int main(int argc, char** argv)
{
    // This is all unimportant, just imagine you have a window that is 600x600 and you can draw on it
    // in normalized [-1,1] coordinates
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Insert cookies here!");

    

    if (gl3wInit()) {
        std::cout << "failed to initialize OpenGL\n";
        return -1;
    }
    if (!gl3wIsSupported(3, 3)) {
        std::cout << "OpenGL 3.3 not supported\n";
        return -1;
    }

    std::cout << "OpenGL\t" << glGetString(GL_VERSION) << '\n' <<
        "GLSL\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    // This is where we prepare our data
    init();

    // This is where we register a function to be called each frame to render stuff
    glutDisplayFunc(display);

    // This is where we start the main loop
    glutMainLoop();

    // Cleanup afterwards
    cleanUp();
    return 0;
}