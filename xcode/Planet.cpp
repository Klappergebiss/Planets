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

Planet::Planet(vec2 pos, dvec2 dir, float speed, int r) {
    mPos = pos;
    mDir = dir;
    mRadius = r;
    mSpeed = speed;
    
    mMass = mRadius * 2 * M_PI;
    mGrav = mMass * 0.000005f;
    mGravRadius = mRadius * 5;
    mForeignGrav = 0.0f;
    mForeignForce = vec2(0,0);
    
    mRed = randFloat(0.05, 0.7);
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
    mGrav = mMass * 0.000003f;
    mGravRadius = mRadius * 5;
}

void Planet::update() {
    if(mRangedPlanets.empty()) {
        setForeignForce(vec2(0,0));
        setForeignGrav(0.0f);
    } else {
        while(!mRangedPlanets.empty()) {
            setForeignGrav(mRangedPlanets.back()->getGrav());
            if(mRangedPlanets.back()->isCollided) {
                invForeignForce(mRangedPlanets.back()->getPos());
            } else setForeignForce(mRangedPlanets.back()->getPos());
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
    setDir(mDir + mForeignForce * mForeignGrav);
    hasMoved = true;
}

void Planet::collide(Planet* somePlanet) {
    mRed = (mRed * mRadius + somePlanet->mRed * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
    mGreen = (mGreen * mRadius + somePlanet->mGreen * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
    mBlue = (mBlue * mRadius + somePlanet->mBlue * somePlanet->getRadius() ) / (mRadius+somePlanet->getRadius());
    updateRadius( mRadius + somePlanet->getRadius() * 0.2f );
    somePlanet->isCollided = true;
    
}

