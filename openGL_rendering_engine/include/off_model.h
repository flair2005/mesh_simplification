#ifndef OFF_MODEL
#define OFF_MODEL
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <glm/glm.hpp>
#include <algorithm>
#include "../include/Window.h"

using namespace std;

class off_model{
    public:
        off_model();
        off_model(const char* path);    //Path constructor
        void draw(GLuint shader);
        void scale(bool);
        void edge_collapse(int v0, int v1); //Edge collapse the vertices at these indexes
        void vertex_split();
        void print_vfa();
        void read_edge_collapses();

    private:
        vector<glm::vec3> vertices;        //Vertices are floats arrays of size 3
        vector<glm::vec3> faces;           //Vertices are in CCW order
        vector<unsigned int> faces_flat;   //Order to draw everything, each face index = 3*index
        vector<vector<unsigned int> > vertex_face_adj;  //Keeps track of all adjacent faces to each vertex
        vector<glm::vec3> normals;         //Vertex normals for shading purposes (and debug)
        vector<string> edge_collapse_string;    //Vector that keeps the edge collapses in memory

        void calculate_normals();          //Helper function for calculating the normals
        void rebuild_flat_faces();         //Helper function to rebuild the flat faces
        void update_vbos();                //Helper function to update the vbos after edge collapses
        void rebuild_vfa();                //Helper function to rebuild the vfa after we screw with it in edge collapse
        void print_faces();                //Helper function to print the face table
        void print_vertices();

        void parse_file(const char* path);  //Parses the off file specified by the path

        GLuint VAO, VBO, EBO, NBO;          //Buffers for opengl
        GLuint uProjection, uModelview;     //Used to send projection and modelview to the shaders
        GLuint uModel;                      //Used for shading
        glm::mat4 toWorld;                  //toWorld matrix

};

#endif
