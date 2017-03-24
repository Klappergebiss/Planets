//
//  PlanetManager.cpp
//  Planets
//
//  Created by Tim Engel on 21.06.16.
//
//

#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "PlanetManager.hpp"

using namespace ci;
using std::list;

#pragma mark - initialize

PlanetManager::PlanetManager()
{
    freq = 99;
    enableCollide = true;
    hasBlackHole = false;
    
}

#pragma mark - update

void PlanetManager::update()
{
    for (list<Star*>::iterator s = mStars.begin(); s != mStars.end(); ++s) {
        (*s)->update();
    }
    
    for( list<Planet*>::iterator p = mPlanets.begin(); p != mPlanets.end(); ++p ){
        if (mPlanets.size() > 1) {
            for( list<Planet*>::iterator q = mPlanets.begin(); q != mPlanets.end(); ++q) {
                isInRange(*p, *q);
                // if planet collided -> removes from the list...
                if ((*q)->isCollided) q = mPlanets.erase(q);
            }
        }
        
        (*p)->update();
        
            // if planet gets too huge, it explodes
        if (!(*p)->isBlackHole && (*p)->getRadius() > 60) {
            (*p)->mRangedPlanets.clear();
            explodePlanet(*p);
            p = mPlanets.erase(p);
        } else if ((*p)->getRadius() > 100) {   //if blackHole, explode at radius > 100
            (*p)->mRangedPlanets.clear();
            explodePlanet(*p);
            p = mPlanets.erase(p);
        }
        
            // if planet leaves screen, it despawns (offset of 80 px for smoother look)
        if ( (*p)->getPos().x < -80 or (*p)->getPos().x > app::getWindowWidth()+80 or (*p)->getPos().y < -80 or (*p)->getPos().y > app::getWindowHeight()+80) {
            (*p)->mRangedPlanets.clear();
            p = mPlanets.erase(p);
        }
    }
    
        // balancing the amount of planets...
    if (mPlanets.size() <= 13 and freq > 21) freq = freq - randInt(-2,5);
    if (randInt(1000001) % (int)freq == 0) addPlanets(1);
    if (mPlanets.size() > 37 and freq < 161) freq = freq + randInt(-4,7);   //21
    if ( !hasBlackHole && (randInt(1000001) % 1301 == 0)) {
        addBlackHole();
        hasBlackHole = true;
    }
}

void PlanetManager::draw()
{
    for( list<Star*>::iterator s = mStars.begin(); s != mStars.end(); ++s ){
        (*s)->draw();
    }
    
    for( list<Planet*>::iterator p = mPlanets.begin(); p != mPlanets.end(); ++p ){
        if ((*p)->hasMoved) (*p)->draw();
    }
}

#pragma mark - add/remove

void PlanetManager::addPlanets( int amt )
{
    for( int i=0; i<amt; i++ )
    {
        float x = randFloat(app::getWindowWidth());
        float y = randFloat(app::getWindowHeight());
        if ( randInt(2)) {
            x = (randFloat(-70,-25) + randInt(2) * (app::getWindowWidth() + 95));
        } else {
            y = (randFloat(-70,-25) + randInt(2) * (app::getWindowHeight() + 95));
        }

        float dirx = randFloat(-1.2, 1.2);  //2.0, 2.0
        float diry = randFloat(-1.2, 1.2);  //2.0, 2.0
        float speed = randFloat(0.2f);
        float rad = randFloat(5.0, 27.0);
        
        Planet* tempPlanet = new Planet( vec2(x, y), vec2(dirx, diry), speed, rad, false);
        mPlanets.push_back(tempPlanet);
    }
}

void PlanetManager::removePlanets( int amt )
{
    for( int i=0; i<amt; i++ )
    {
        mPlanets.pop_back();
    }
}

void PlanetManager::addStars( int amt ) {
    for( int i=0; i<amt; i++) {
        Star* tempStar = new Star();
        mStars.push_back(tempStar);
    }
}

void PlanetManager::addBlackHole() {
    float x = (app::getWindowWidth()*0.5f) + randFloat(-30,30);
    float y = (app::getWindowHeight()*0.5f) + randFloat(-20,20);
    
    float dirx = 0.0f;
    float diry = 0.0f;
    float speed = 0.0f;
    float rad = 15.0f;
    
    Planet* tempPlanet = new Planet( vec2(x, y), vec2(dirx, diry), speed, rad , true);
    mPlanets.push_back(tempPlanet);

}

#pragma mark - planetary actions

void PlanetManager::isInRange(Planet* planet1, Planet* planet2) {
    if(distance(planet1->getPos(), planet2->getPos()) != 0) {     //wenn planet1 != planet2
                    //planet1 in planet2.gravRadius
        if (enableCollide) {   //planet2 is planet && touches planet1
            if (!planet2->isBlackHole && (planet1->getRadius() > distance(planet1->getPos(), planet2->getPos()) ) ) {
                planet1->collide(planet2);  //planet 2 collides into planet1
            }
        }
             //planet1 is in gravRad of planet2 AND planet1 is a planet
        if (!planet1->isBlackHole && (planet2->getGravRadius() > distance(planet1->getPos(), planet2->getPos()))) {
            planet1->mRangedPlanets.push_back(planet2);
        }
    }
}

void PlanetManager::explodePlanet (Planet* planet) {
    vec2 oldPos = planet->getPos();
    float oldRad = planet->getRadius()*0.6;
    int amt = randInt(7,17);        // 15
    float newRad = 2.2*planet->getRadius()/(float)amt;
    
    for (int i = 1; i<=amt; i++) {
        float x = oldPos.x;
        float y = oldPos.y;
        switch (i%4) {
            case 0:
                x += i * (oldRad/(float)amt) + randFloat(-5.0,5.0);;
                y -= sqrt(pow(oldRad,2) - pow((x - oldPos.x),2)) + randFloat(-5.0,5.0);;
                break;
            case 1:
                x -= i * (oldRad/(float)amt) + randFloat(-5.0,5.0);;
                y += sqrt(pow(oldRad,2) - pow((x - oldPos.x),2)) + randFloat(-5.0,5.0);;
                break;
            case 2:
                x -= i * (oldRad/(float)amt) + randFloat(-5.0,5.0);;
                y -= sqrt(pow(oldRad,2) - pow((x - oldPos.x),2)) + randFloat(-5.0,5.0);;
                break;
            default:
                x += i * (oldRad/(float)amt) + randFloat(-5.0,5.0);;
                y += sqrt(pow(oldRad,2) - pow((x - oldPos.x),2)) + randFloat(-5.0,5.0);;
                break;
        }

        float dirx = (x - oldPos.x)*randFloat(0.2,0.25);
        float diry = (y - oldPos.y)*randFloat(0.2,0.25);
        float speed = 0.3;
        newRad += randFloat(-3.0,4.0);
        
        Planet* tempPlanet = new Planet( vec2(x, y), vec2(dirx, diry), speed, newRad, false );
        mPlanets.push_back(tempPlanet);
    }
}

