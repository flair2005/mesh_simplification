#include "../include/light.h"

light::light(GLuint shader){
    position = vec3(1.0f,1.0f,1.0f);
    color    = vec4(.98f,.98f,.25, 1.0f);
    direction= vec3(0.0f,0.0f,0.0f);
    intensity= vec3(1.0f,1.0f,1.0f);
    directional = true;
    shader_program = shader;
}

light::light(GLuint shader, vec3 pos, vec3 col, vec3 dir, vec3 inten,bool dir){
    position = pos;
    color    = col;
    direction= dir;
    intensity= inten;
    directional = dir;
    shader_program = shader;
}


