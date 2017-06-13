//
//  scene.cpp
//  openGL_rendering_engine
//
//  Created by Freddy Sauceda on 6/2/17.
//  Copyright © 2017 Alfredo Sauceda. All rights reserved.
//

#include "../include/scene.hpp"

scene::scene(){}

void scene::draw(){
    for(auto object : scene_contents){
        object->draw();
    }
}

scene::~scene(){
    scene_contents.clear();
}

//Note, the skybox is ALWAYS added first then kept as the last element
int scene::add_object(object* obj){
    size_t size = scene_contents.size(); //Size = index of last element + 1
    if(size == 0) {
        //First object, must be skybox for optimizations to work
        scene_contents.push_back(obj);
        return (int)size;
    }
    else {
        object* skybox = scene_contents[scene_contents.size()-1];
        scene_contents.pop_back(); //Removes the skybox from the end
        scene_contents.push_back(obj);    //We add an element so size = index of it
        scene_contents.push_back(skybox);
        return (int)scene_contents.size()-2;
    }
}
