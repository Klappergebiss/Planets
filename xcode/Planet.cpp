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

#pragma mark - Planet Instance

Planet::Planet(vec2 pos, vec2 dir, float speed, int r, bool thisIsBlackHole) {
    mPos = pos;
    mDir = dir;
    mRadius = r;
    mSpeed = speed;
    
    mForeignGrav = 0.0f;
    mForeignForce = vec2(0,0);
    
    hasMoved = false;
    isCollided = false;
    
    radius(mRadius);
    isBlackHole = thisIsBlackHole;
    
    if(!thisIsBlackHole) {
        mMass = mRadius * 4.0; // mRadius * M_PI * 2
        mGrav = mMass * 0.00001f;   // 0.000003
        mGravRadius = mRadius * 7;  //5
        
        mRed = randFloat(0.05, 0.7);
        mGreen = randFloat(0.05, 0.7);
        mBlue = randFloat(0.05, 0.5);
        
        subdivisions(53);
    } else {
        mMass = mRadius * 4.0; // mRadius * M_PI * 2
        mGrav = mMass * 0.00007f;    // 0.00002f
        mGravRadius = mRadius * 12;
        
        mRed = 0.0f;
        mGreen = 0.0f;
        mBlue = 0.0f;
        
        subdivisions(23);
    }
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
    
    delete &mRed;
    delete &mGreen;
    delete &mBlue;
    
    mRangedPlanets.clear();
}

#pragma mark - get-methods

vec2 Planet::getPos() {
    return mPos;
}

vec2 Planet::getDir() {
    return mDir;
}

int Planet::getGravRadius() {
    return mGravRadius;
}

float Planet::getGrav() {
    return mGrav;
}

int Planet::getRadius() {
    return mRadius;
}

float Planet::getSpeed() {
    return mSpeed;
}

#pragma mark - set-methods

void Planet::setPos(vec2 pos) {
    mPos = pos;
    center(mPos);
}

void Planet::setDir(vec2 dir) {
    mDir = dir;
}

    // to push bigger planet into direction of collided one
void Planet::invForeignForce(vec2 someForce) {
    mForeignForce += (mPos - someForce) * mForeignGrav * 14.0f;    //(ohne 70.0f) evtl 200 od 50 ??
}

void Planet::setForeignForce(vec2 someForce) {
    mForeignForce += (someForce - mPos) * mForeignGrav*0.2f;    // (ohne)
}

void Planet::setForeignGrav(float someGrav) {
    mForeignGrav = someGrav;
}

void Planet::setSpeed(float someSpeed) {
    mSpeed = someSpeed;
}

#pragma mark - update

void Planet::updateRadius(int newRadius) {
    mRadius = newRadius;
    radius(mRadius);
    mMass = mRadius * 4.0; // * 2
    
    if(!isBlackHole) {
        mGrav = mMass * 0.00001f;   // 0.000003f
        mGravRadius = mRadius * 7;  //5
    } else {
        mGrav = mMass * 0.00004f;    // 0.00002
        mGravRadius = mRadius * 12; //9
    }
}

void Planet::update() {
    if(mRangedPlanets.empty()) {
        setForeignForce(vec2(0,0));
        setForeignGrav(0.0f);
    } else {    // if there are influencing planets
        while(!mRangedPlanets.empty()) {
            
            Planet* otherPlanet = mRangedPlanets.back();    //new
            float normedDist = (distance(otherPlanet->getPos(), this->getPos()) - otherPlanet->getRadius()) / (otherPlanet->getGravRadius() - otherPlanet->getRadius());    //new
            if (!isBlackHole) { // if planet then get attracted
//                setForeignGrav(mRangedPlanets.back()->getGrav());
                setForeignGrav( (otherPlanet->getGrav()) * cos(1.5 * pow(normedDist,2)));    //cosine
//                setForeignGrav( (otherPlanet->getGrav()) * (-normedDist+1));    //linear
                
                
                if(mRangedPlanets.back()->isCollided) {
                    invForeignForce(mRangedPlanets.back()->getPos());
                } else setForeignForce(mRangedPlanets.back()->getPos());
            } else {
//                mGrav = getGrav() + 0.00001f;     //let grow grav of blackhole constantly
                setForeignGrav(0.0f);
                if(mRangedPlanets.back()->isCollided) {
                    invForeignForce(vec2(0,0));
                } else setForeignForce(vec2(0,0));
            }
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
    if(!isBlackHole) {
        setPos(mPos + mDir*mSpeed + mForeignForce);
        setDir(mDir + mForeignForce * mForeignGrav);    //mDir*0.7f + mForeignForce * mForeignGrav
        hasMoved = true;                                //looks cool; but stops pieces after explosion
    } else {
        setPos(mPos);
        setDir(mDir);
        hasMoved = true;
    }
}

void Planet::collide(Planet* somePlanet) {
    if(!isBlackHole) {
        mRed = (mRed * mRadius + somePlanet->mRed * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
        mGreen = (mGreen * mRadius + somePlanet->mGreen * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
        mBlue = (mBlue * mRadius + somePlanet->mBlue * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
        
        updateRadius( mRadius + somePlanet->getRadius() * 0.2f );
    } else { // if blackhole
        updateRadius( mRadius + somePlanet->getRadius() * 0.1f );
    }
    somePlanet->isCollided = true;
    
}

