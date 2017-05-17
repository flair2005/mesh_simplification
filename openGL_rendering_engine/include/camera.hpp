//
//  camera.hpp
//  openGL_rendering_engine
//
//  Created by Freddy Sauceda on 5/16/17.
//  Copyright © 2017 Alfredo Sauceda. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>
#include "../include/Window.h"

using namespace glm;

class camera{
    public:
        camera(vec3 pos, vec3 look, vec3 up);
        
        void rotate(double, double, double, double);
        
        vec3 get_pos();
        vec3 get_look_at();
        vec3 get_up();

    private:
        vec3 cam_pos;
        vec3 look_at;
        vec3 cam_up;

};

#endif /* camera_hpp */
