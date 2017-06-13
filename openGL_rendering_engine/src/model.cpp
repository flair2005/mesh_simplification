#include "../include/model.hpp"

model::model(off_model inner, GLuint shade){
    toWorld = glm::mat4(1.0f);
    shader = shade;
    
    vertices = inner.get_vert();
    faces = inner.get_faces();
    normals = inner.get_normals();
    
    //Create the buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*sizeof(int), &faces[0], GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*sizeof(glm::vec3), &faces_flat[0], GL_STATIC_DRAW);
    
    //Bind the normals
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

model::~model(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &NBO);
}

void model::draw(){
    glUseProgram(shader);
    //Calculate the model and view (cam inverse) matrix
    glm::mat4 modelview = Window::V * toWorld;
    
    //Forward variables to the shader program
    uModel      = glGetUniformLocation(shader, "model");
    uProjection = glGetUniformLocation(shader, "projection");
    uModelview  = glGetUniformLocation(shader, "modelview");
    
    //Actually send those variables now
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &this->toWorld[0][0]);
    
    //Actually draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)faces.size(), GL_UNSIGNED_INT, 0);
}

void model::scale(float x, float y, float z){
    glm::mat4 scale = glm::scale(mat4(1.0f), glm::vec3(x,y,z));
    toWorld = scale * toWorld;
}
