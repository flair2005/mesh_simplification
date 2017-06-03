//
//  scene.hpp
//  openGL_rendering_engine
//
//  Created by Freddy Sauceda on 6/2/17.
//  Copyright © 2017 Alfredo Sauceda. All rights reserved.
//

#ifndef scene_hpp
#define scene_hpp

#include <stdio.h>
#include <vector>
#include "../include/object.hpp"
#include "../include/Window.h"

class scene{

public:
    scene();
    void draw();
    
private:
    vector<object> scene_contents;
};

#endif /* scene_hpp */
