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
    mGrav = mMass * 0.000003f;    //0.00001f;
    mGravRadius = mRadius * 5;  //5
    mForeignGrav = 0.0f;
    mForeignForce = vec2(0,0);
    mForeignDist = 0.0f;
    
    mRed = randFloat(0.05, 0.6);
    mGreen = randFloat(0.05, 0.7);
    mBlue = randFloat(0.05, 0.5);
    
    hasMoved = false;
    isCollided = false;
    
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

void Planet::updateRadius(int newRadius) {
    mRadius = newRadius;
    radius(mRadius);
    mMass = mRadius * 2 * M_PI;
    mGrav = mMass * 0.000003f;
    mGravRadius = mRadius * 5;
}

int Planet::getRadius() {
    return mRadius;
}

void Planet::update() {
    if(mRangedPlanets.empty()) {
        setForeignForce(vec2(0,0));
        setForeignGrav(0.0f);
    } else {
        while(!mRangedPlanets.empty()) {
            setForeignGrav(mRangedPlanets.back()->getGrav());
            setForeignForce(mRangedPlanets.back()->getPos());
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
    center(mPos);
    setDir(mDir + mForeignForce*mForeignGrav);
    hasMoved = true;
}

void Planet::collide(Planet* somePlanet) {
    mRed = (mRed * mRadius + somePlanet->mRed * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
    mGreen = (mGreen * mRadius + somePlanet->mGreen * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
    mBlue = (mBlue * mRadius + somePlanet->mBlue * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
    updateRadius( mRadius + somePlanet->getRadius() * 0.2f );
    somePlanet->isCollided = true;
    
}




//rangedPlanets wird net wirklich gefüllt....
//weil nur referenzen in listen bearbeitet werden!!