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

PlanetManager::PlanetManager()
{
}

void PlanetManager::update()
{
    for( list<Planet*>::iterator p = mPlanets.begin(); p != mPlanets.end(); ++p ){
        if (mPlanets.size() > 1) {
            for( list<Planet*>::iterator q = mPlanets.begin(); q != mPlanets.end(); ++q) {
                isInRange(*p, *q);
                //            cout<<"p1: "<<&p<<"\n\tq2: "<<&q<<endl;
            }
        }
        
        (*p)->update();
        
        if ( (*p)->getPos().x < -50 or (*p)->getPos().x > app::getWindowWidth()+50 or (*p)->getPos().y < -50 or (*p)->getPos().y > app::getWindowHeight()+50) {
            (*p)->mRangedPlanets.clear();
            p = mPlanets.erase(p);
        }
    }
    if (mPlanets.size() <= 1) addPlanets(randInt(18,27)); //180,230 sind gut
}

void PlanetManager::draw()
{
    for( list<Planet*>::iterator p = mPlanets.begin(); p != mPlanets.end(); ++p ){
        (*p)->draw();
    }
}

void PlanetManager::addPlanets( int amt )
{
    for( int i=0; i<amt; i++ )
    {
        float x = randFloat( app::getWindowWidth() );
        float y = randFloat( app::getWindowHeight() );
        float dirx = randFloat(-2.0, 2.0);
        float diry = randFloat(-2.0, 2.0);
        float speed = randFloat(1.0f);
        float rad = randFloat(5.0, 45.0);
//        mPlanets.push_back( Planet( vec2(x, y), dvec2(dirx, diry), speed, rad ) );
        
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
        
//        cout<<"distnace: "<<distance(planet1.getPos(), planet2.getPos())<<endl;
//        cout<<"gravRad: "<<planet2.getGravRadius()<<endl;
//        cout<<"planet1: "<<planet1<<"\tplanet2:"<<planet2<<endl;
                                //planet1 in planet2.gravRadius
        if (planet2->getGravRadius() > distance(planet1->getPos(), planet2->getPos())) {
            planet1->mRangedPlanets.push_back(planet2);      //planet1 wird von planet2 angezogen
//            cout<<"yep"<<endl;
        
//        planet1.setForeignForce(planet2.getPos());    // yup...fertig!
//        planet1.setForeignGrav(planet2.getGrav());    //* noch scalieren mit π usw.....
        }
    }
}
















