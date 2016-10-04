//
//  Star.hpp
//  Planets
//
//  Created by Tim Engel on 03.10.16.
//
//

#ifndef Star_hpp
#define Star_hpp

#include <stdio.h>
#include "cinder/GeomIo.h"

using namespace ci;
using namespace ci::geom;
using namespace std;

class Star : public Circle {
    vec2 mPos;
    vec2 mDirection;
    float mRed, mGreen, mBlue;
    
public:
    Star();
    
    vec2 getPos();
    void setPos(vec2 pos);
    void move();
    
    void update();
    void draw();
};


#endif /* Star_hpp */
