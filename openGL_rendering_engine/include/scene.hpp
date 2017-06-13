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
#include "object.hpp"

using namespace std;

class scene{
public:
    scene();
    ~scene();
    void draw();
    int add_object(object*);    //Returns index of this object
    
private:
    vector<object*> scene_contents;
};

#endif /* scene_hpp */
