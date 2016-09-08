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
#include "cinder/Timer.h"
#include "PlanetManager.hpp"

using namespace ci;
using std::list;

PlanetManager::PlanetManager()
{
    freq = 99;
}

void PlanetManager::update()
{
    for( list<Planet*>::iterator p = mPlanets.begin(); p != mPlanets.end(); ++p ){
        if (mPlanets.size() > 1) {
            for( list<Planet*>::iterator q = mPlanets.begin(); q != mPlanets.end(); ++q) {
                isInRange(*p, *q);
                if ((*q)->isCollided) q = mPlanets.erase(q);
            }
        }
        
        (*p)->update();
        
        if ( (*p)->getPos().x < -80 or (*p)->getPos().x > app::getWindowWidth()+80 or (*p)->getPos().y < -80 or (*p)->getPos().y > app::getWindowHeight()+80) {
            (*p)->mRangedPlanets.clear();
            p = mPlanets.erase(p);
        }
    }
    if (mPlanets.size() <= 4 and freq > 21) freq = freq - randInt(-2,5);
    if (randInt(1000001) % (int)freq == 0) addPlanets(1);
//    cout << "freq: " << freq << endl;
    if (mPlanets.size() > 31 and freq < 161) freq = freq + randInt(-4,7);   //21
}

void PlanetManager::draw()
{
    for( list<Planet*>::iterator p = mPlanets.begin(); p != mPlanets.end(); ++p ){
        if ((*p)->hasMoved) (*p)->draw();
    }
}

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
                    // randFloat(app::getWindowHeight());
        float dirx = randFloat(-1.2, 1.2);  //2.0, 2.0
        float diry = randFloat(-1.2, 1.2);  //2.0, 2.0
        float speed = randFloat(1.0f);
        float rad = randFloat(5.0, 27.0);
        
//        float x = app::getWindowWidth() * (i+1)/8.0;
//        float y = app::getWindowHeight() * (i+1)/7.0;
//        float dirx = -i-0.01;
//        float diry = 0;
//        float speed = 0.1f;
//        float rad = 5*(i+1);
        Planet* tempPlanet = new Planet( vec2(x, y), dvec2(dirx, diry), speed, rad );
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

void PlanetManager::isInRange(Planet* planet1, Planet* planet2) {
    if(distance(planet1->getPos(), planet2->getPos()) != 0) {     //wenn planet1 != planet2
                    //planet1 in planet2.gravRadius
        if (planet1->getRadius() > distance(planet1->getPos(), planet2->getPos()) ) {
            planet1->collide(planet2);
        } else if (planet2->getGravRadius() > distance(planet1->getPos(), planet2->getPos())) {
            planet1->mRangedPlanets.push_back(planet2);      //planet1 wird von planet2 angezogen
        }
    }
}
















