//
//  Planet.hpp
//  Planets
//
//  Created by Tim Engel on 10.06.16.
//
//

#ifndef Planet_hpp
#define Planet_hpp

#include <stdio.h>
#include "cinder/GeomIo.h"
#include <list>

using namespace ci;
using namespace ci::geom;
using namespace std;

class Planet : public Circle {

    vec2 mPos;          // my position
    vec2 mDir;          // my direction
    float mSpeed;       // my speed
    int mRadius;        // my radius
    int mMass;          // my mass
    float mGrav;        // my gravity
    int mGravRadius;    // my grav radius
    
    float mForeignGrav; // some gravity
    vec2 mForeignForce; // direction of foreign grav
    float mForeignDist; // distance to foreign planet
    float mRed, mGreen, mBlue;         // colors of planet to be drawn
    

public:
    Planet();
    Planet(vec2 pos, dvec2 dir, float speed, int r);
    ~Planet();
    vec2 getPos();
    void setPos(vec2 pos);
    vec2 getDir();
    void setDir(vec2 dir);
    float getGrav();
    vec2 getForeignForce();
    void setForeignForce(vec2 someForce);
    float getForeignGrav();
    void setForeignGrav(float someGrav);
    int getGravRadius();
    bool hasMoved;
    
    std::list<Planet*>	mRangedPlanets; // planets in range to gravitate
    
    void update();
    void draw();
    
    void move();
    
};


#endif /* Planet_hpp */
