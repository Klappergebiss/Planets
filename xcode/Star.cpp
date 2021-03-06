//
//  Star.cpp
//  Planets
//
//  Created by Tim Engel on 03.10.16.
//
//

#include "Star.hpp"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

Star::Star() {
    mPos = vec2(randFloat(-app::getWindowWidth()*0.5,app::getWindowWidth()*1.5), randFloat(-app::getWindowHeight()*0.5,app::getWindowHeight()*1.5));
    
    mDirection = app::getWindowCenter()+vec2(randFloat(-50,50),randFloat(-50,50));
    mRed = 0.5;
    mGreen = 0.5;
    mBlue = 0.5;
    
    int whichColor = randInt(5);
    switch (whichColor) {
        case 0:
            mRed = mRed + randFloat(0.07, 0.19);    //red
            break;
        case 1:
            mGreen = mGreen + randFloat(0.07, 0.19);    //green
            break;
        case 2:
            mRed = mRed + randFloat(0.07,0.19);     //yellow
            mGreen = mRed;
            break;
        case 3:
            mGreen = mGreen + randFloat(0.07,0.19); //turkis
            mBlue = mGreen;
        default:
            mBlue = mBlue + randFloat(0.07, 0.19);  //blue
            break;
    }
    
    radius(randFloat(1,4));
    subdivisions(randInt(3,7));
    center(mPos);
}

vec2 Star::getPos() {
    return mPos;
}

void Star::setPos(vec2 pos) {
    mPos = pos;
}

void Star::move() {
    vec2 direction = getPos() - mDirection;
    direction.x = direction.x / app::getWindowWidth();
    direction.y = direction.y / app::getWindowHeight();

    float alpha = M_PI/2.0f;   //app::getElapsedFrames()
    setPos(mPos + vec2(((cos(alpha) * direction.x - sin(alpha) * direction.y)*0.2), ((sin(alpha) * direction.x + cos(alpha) * direction.y)*0.2)));
    
//    setPos(mPos + vec2(sin(alpha)*direction.y*0.2, -sin(alpha)*direction.x*0.2));
    center(mPos);
}

void Star::update() {
    move();
}

void Star::draw() {
    gl::color(mRed, mGreen, mBlue);
    gl::draw(*this);

}
