#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/GeomIo.h"
#include "cinder/Rand.h"
#include "Planet.hpp"
#include "PlanetManager.hpp"
#include <ctime>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace ci::geom;

class PlanetsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
//    Planet *myPlanet;
//    Planet *somePlanet;
    
    PlanetManager mPlanetManager;
    
};

void PlanetsApp::setup()
{
    time_t currentTime;
    time(&currentTime);
    
    randSeed((double)currentTime);
    setWindowPos(250, 50);
    setWindowSize(700, 700);
    
    mPlanetManager.addPlanets(25);
}

void PlanetsApp::mouseDown( MouseEvent event )
{
}

void PlanetsApp::update()
{
    mPlanetManager.update();
    
    /*
    // wenn:
    if (somePlanet->getGravRadius() > sqrt( pow( ((somePlanet->getPos() - myPlanet->getPos()).x), 2) + pow( ((somePlanet->getPos() - myPlanet->getPos()).y), 2) ) ) {
        myPlanet->setForeignForce(somePlanet->getPos());
        myPlanet->setForeignGrav(somePlanet->getGrav());    // noch scalieren mit π usw.....
    } else {
        myPlanet->setForeignGrav(0.0f);
    }
    myPlanet->move();
     */
}

void PlanetsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    mPlanetManager.draw();
//    gl::color(0.09f,0.22f,0.001f);
//    gl::draw(*myPlanet);
//    gl::color(0.32f, 0.02f, 0.001f);
//    gl::draw(*somePlanet);
}

CINDER_APP( PlanetsApp, RendererGl )







