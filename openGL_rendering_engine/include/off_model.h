#ifndef OFF_MODEL
#define OFF_MODEL
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <glm/glm.hpp>
#include "../include/Window.h"

using namespace std;

class off_model{
    public:
        off_model();
        off_model(const char* path);    //Path constructor
        void draw(GLuint shader);
        void scale(bool);

    private:
        vector<glm::vec3> vertices;        //Vertices are floats arrays of size 3
        vector<glm::vec3> faces;           //Vertices are in CCW order
        vector<unsigned int> faces_flat;   //Order to draw everything, each face index = 3*index
        vector<vector<unsigned int> > vertex_face_adj;  //Keeps track of all adjacent faces to each vertex
        vector<glm::vec3> normals;         //Vertex normals for shading purposes (and debug)

        void calculate_normals();          //Helper function for calculating the normals

        void parse_file(const char* path);  //Parses the off file specified by the path

        GLuint VAO, VBO, EBO, NBO;          //Buffers for opengl
        GLuint uProjection, uModelview;     //Used to send projection and modelview to the shaders
        GLuint uModel;                      //Used for shading
        glm::mat4 toWorld;                  //toWorld matrix

};

#endif
