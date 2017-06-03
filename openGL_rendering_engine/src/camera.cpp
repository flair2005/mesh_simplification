//
//  camera.cpp
//  openGL_rendering_engine
//
//  Created by Freddy Sauceda on 5/16/17.
//  Copyright © 2017 Alfredo Sauceda. All rights reserved.
//

#include "../include/camera.hpp"

camera::camera(vec3 pos, vec3 look, vec3 up){
    this->cam_pos = pos;
    this->look_at = look;
    this->cam_up = up;
}

void camera::rotate(double last_x, double last_y, double curr_xpos, double curr_ypos){
    float angle;
    //X axis rotation
    angle = (float)(last_x - curr_xpos)/100.0f;
    this->cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_pos, 1.0f));
    this->cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_up, 1.0f));
    //Y axis rotation
    angle = (float)(last_y - curr_ypos)/100.0f;
    glm::vec3 axis = glm::cross((cam_pos - look_at), cam_up);
    this->cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_pos, 1.0f));
    this->cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_up, 1.0f));

}

vec3 camera::get_pos(){
    return cam_pos;
}

vec3 camera::get_look_at(){
    return look_at;
}

vec3 camera::get_up(){
    return cam_up;
}


