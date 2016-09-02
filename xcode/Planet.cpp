//
//  Planet.cpp
//  Planets
//
//  Created by Tim Engel on 10.06.16.
//
//

#include "Planet.hpp"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

using std::list;

Planet::Planet() {
    mPos = vec2(randFloat(700), randFloat(700));
    mDir = randVec2() * randFloat(1,5);
    mRadius = randInt(1,40);
    mSpeed = randFloat(2.5f);
    
    mMass = mRadius * 2 * M_PI;
    mGrav = mMass * 0.0025f;
    mGravRadius = mRadius * 2;
    mForeignGrav = 0.0f;
    mForeignForce = vec2(0,0);
    mForeignDist = 0.0f;
    
    mRed = randFloat(0.4);
    mGreen = randFloat(0.8);
    mBlue = randFloat(0.5);
    
    radius(mRadius);
    subdivisions(100);
    
}

Planet::Planet(vec2 pos, dvec2 dir, float speed, int r) {
    mPos = pos;
    mDir = dir;
    mRadius = r;
    mSpeed = speed;
    
    mMass = mRadius * 2 * M_PI;
    mGrav = mMass * 0.000001f;    //0.0001f;
    mGravRadius = mRadius * 5;
    mForeignGrav = 0.0f;
    mForeignForce = vec2(0,0);
    mForeignDist = 0.0f;
    
    mRed = randFloat(0.6);
    mGreen = randFloat(0.7);
    mBlue = randFloat(0.5);
    
    radius(mRadius);
    subdivisions(100);
}

Planet::~Planet() {
    delete &mDir;
    delete &mPos;
    delete &mRadius;
    delete &mSpeed;
    
    delete &mMass;
    delete &mGrav;
    delete &mGravRadius;
    delete &mForeignGrav;
    delete &mForeignForce;
    delete &mForeignDist;
    
    delete &mRed;
    delete &mGreen;
    delete &mBlue;
    
    mRangedPlanets.clear();
    

}

vec2 Planet::getPos() {
    return mPos;
}

void Planet::setPos(vec2 pos) {
    mPos = pos;
}

vec2 Planet::getDir() {
    return mDir;
}

void Planet::setDir(vec2 dir) {
    mDir = dir;
}

float Planet::getGrav() {
    return mGrav;
}

void Planet::setForeignForce(vec2 someForce) {
    mForeignForce += (someForce - getPos()) * getForeignGrav();
}

vec2 Planet::getForeignForce() {
    return mForeignForce;
}

void Planet::setForeignGrav(float someGrav) {
    mForeignGrav = someGrav;
}

float Planet::getForeignGrav() {
    return mForeignGrav;
}

int Planet::getGravRadius() {
    return mGravRadius;
}

void Planet::update() {
    if(mRangedPlanets.empty()) {
        setForeignForce(vec2(0,0));
        setForeignGrav(0.0f);
    } else {
        while(!mRangedPlanets.empty()) {
            setForeignGrav(mRangedPlanets.back()->getGrav());//-0.000001f);
            setForeignForce(mRangedPlanets.back()->getPos()); // * mRangedPlanets.back()->getGrav());
            mRangedPlanets.pop_back();
        }
    }
    
    move();
}

void Planet::draw() {
    gl::color(mRed, mGreen, mBlue);
    gl::draw(*this);
}


void Planet::move() {
    setPos(mPos + mDir*mSpeed + mForeignForce);
//    setPos(mPos + mDir*mSpeed + mForeignForce*mForeignGrav);
    center(mPos);
    setDir(mDir + mForeignForce*mForeignGrav);
//    setDir(mDir + mForeignForce*mForeignGrav);
}




//rangedPlanets wird net wirklich gefüllt....
//weil nur referenzen in listen bearbeitet werden!!