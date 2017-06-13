#ifndef model_hpp
#define model_hpp

#include <stdio.h>
#include <vector>
#include "object.hpp"
#include "off_model.h"

using namespace std;

class off_model;

class model : public object{
public:
    //pretty hacky but this will be moderately faster
    model(off_model inner_model, GLuint shade);
    ~model();
    void draw();
    void scale(float x, float y, float z);
    
private:
    vector<glm::vec3> vertices;        //Vertices are floats arrays of size 3
    vector<glm::vec3> normals;         //Vertex normals for shading purposes (and debug)
    vector<unsigned int> faces;           //Vertices are in CCW order (this is flat_faces)
    
    GLuint shader;                      //Shader program used to draw this
    GLuint VAO, VBO, EBO, NBO;          //Buffers for opengl
    GLuint uProjection, uModelview;     //Used to send projection and modelview to the shaders
    GLuint uModel;                      //Used for shading
    glm::mat4 toWorld;                  //toWorld matrix
    
};

#endif
