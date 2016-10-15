//
//  PlanetManager.hpp
//  Planets
//
//  Created by Tim Engel on 21.06.16.
//
//

#pragma once
#include "Planet.hpp"
#include "Star.hpp"
#include <list>

class PlanetManager {
public:
    PlanetManager();
    void update();
    void draw();
    void addPlanets( int amt );
    void removePlanets( int amt );
    void addStars( int amt );
    void isInRange(Planet* planet1, Planet* planet2);
    void explodePlanet(Planet* planet);
    
    std::list<Planet*>	mPlanets;
    std::list<Star*>    mStars;
    
    int freq;
    bool enableCollide;
};
