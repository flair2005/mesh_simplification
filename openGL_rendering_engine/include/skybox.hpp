#ifndef skybox_hpp
#define skybox_hpp

#include <stdio.h>
#include <string>
#include "object.hpp"
#include "Window.h"

#include "stb_image.h"

using namespace std;

class skybox : public object{
public:
    skybox(vector<string> paths, GLuint shade);
    ~skybox();
    void draw();
    
private:
    glm::mat4 toWorld;
    
    GLuint texture_id;
    vector<string> paths;
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO, shader;
    GLuint uProjection, uModelview;
    
    const GLfloat vertices[8][3] = {
        // "Front" vertices
        {-2.0, -2.0,  2.0}, {2.0, -2.0,  2.0}, {2.0,  2.0,  2.0}, {-2.0,  2.0,  2.0},
        // "Back" vertices
        {-2.0, -2.0, -2.0}, {2.0, -2.0, -2.0}, {2.0,  2.0, -2.0}, {-2.0,  2.0, -2.0}
    };
    const GLuint indices[6][6] = {
        // Front face
        {0, 1, 2, 2, 3, 0},
        // Top face
        {1, 5, 6, 6, 2, 1},
        // Back face
        {7, 6, 5, 5, 4, 7},
        // Bottom face
        {4, 0, 3, 3, 7, 4},
        // Left face
        {4, 5, 1, 1, 0, 4},
        // Right face
        {3, 2, 6, 6, 7, 3}
    };
};

#endif
