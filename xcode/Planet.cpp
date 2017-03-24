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
    
    if(!thisIsBlackHole) {
        mMass = mRadius * 2 * M_PI;
        mGrav = mMass * 0.000003f;
        mGravRadius = mRadius * 5;
        
        mRed = randFloat(0.05, 0.7);
        mGreen = randFloat(0.05, 0.7);
        mBlue = randFloat(0.05, 0.5);
        
        subdivisions(100);
    } else {
        mMass = mRadius * 2 * M_PI;
        mGrav = mMass * 0.00003f;
        mGravRadius = mRadius * 9;
        
        mRed = 0.1f;
        mGreen = 0.1f;
        mBlue = 0.1f;
        
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

#pragma mark - set-methods

void Planet::setPos(vec2 pos) {
    mPos = pos;
}

void Planet::setDir(vec2 dir) {
    mDir = dir;
}

    // to push bigger planet into direction of collided one
void Planet::invForeignForce(vec2 someForce) {
    mForeignForce += (mPos - someForce) * mForeignGrav * 70.0f;    //evtl 200 od 50 ??
}

void Planet::setForeignForce(vec2 someForce) {
    mForeignForce += (someForce - mPos) * mForeignGrav;
}

void Planet::setForeignGrav(float someGrav) {
    mForeignGrav = someGrav;
}

#pragma mark - update

void Planet::updateRadius(int newRadius) {
    mRadius = newRadius;
    radius(mRadius);
    mMass = mRadius * 2 * M_PI;
    
    if(!isBlackHole) {
        mGrav = mMass * 0.000003f;
        mGravRadius = mRadius * 5;
    } else {
        mGrav = mMass * 0.00003f;
        mGravRadius = mRadius * 9;
    }
}

void Planet::update() {
    if(mRangedPlanets.empty()) {
        setForeignForce(vec2(0,0));
        setForeignGrav(0.0f);
    } else {
        while(!mRangedPlanets.empty()) {
                // if planet and planet
            if (!isBlackHole) { // if planet then get attracted
                setForeignGrav(mRangedPlanets.back()->getGrav());
                if(mRangedPlanets.back()->isCollided) {
                    invForeignForce(mRangedPlanets.back()->getPos());
                } else setForeignForce(mRangedPlanets.back()->getPos());
            } else {
                setForeignGrav(0.0f);
                if(mRangedPlanets.back()->isCollided) {
                    invForeignForce(vec2(0,0));
                } else setForeignForce(vec2(0,0));
//                mRangedPlanets.pop_back();
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
        center(mPos);
        setDir(mDir + mForeignForce * mForeignGrav);
        hasMoved = true;
    } else {
        setPos(mPos);
        center(mPos);
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
        mRed = 0.95f;
        mGreen = mRed;
        mBlue = mRed;
        updateRadius( mRadius + somePlanet->getRadius() * 0.1f );
    }
    somePlanet->isCollided = true;
    
}

