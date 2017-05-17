#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Window.h"
#include <stdio>

using namespace glm;

class light{
    public:
        light(GLuint shader);
        light(GLuint shader, vec3 pos, vec3 col, vec3 dir, vec3 inten,bool dir);

    private:
        GLuint shader_program;

        bool directional;
        vec3 position;
        vec4 color;
        vec3 direction;
        vec3 intensity;
        
}

#endif 
