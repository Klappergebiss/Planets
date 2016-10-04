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
	void update() override;
	void draw() override;
    void keyDown( KeyEvent event) override;
    
//    Planet *myPlanet;
//    Planet *somePlanet;
    
    PlanetManager mPlanetManager;
    
};

void PlanetsApp::setup()
{
    time_t currentTime;
    time(&currentTime);
    
    randSeed((double)currentTime);
    setWindowPos(0, 0);
    setWindowSize(getDisplay()->getWidth(), getDisplay()->getHeight());
    setFullScreen(true);
    
    mPlanetManager.addStars(120);
    
    mPlanetManager.addPlanets(5);  //5
    hideCursor();
}


void PlanetsApp::update()
{
    mPlanetManager.update();
    
}

void PlanetsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    mPlanetManager.draw();
}

void PlanetsApp::keyDown( KeyEvent event) {
    if (event.getCode() == KeyEvent::KEY_ESCAPE) {
        setFullScreen(false);
        showCursor();
    }
    if (event.getCode() == 'c') mPlanetManager.isCollide = !mPlanetManager.isCollide;
}

CINDER_APP( PlanetsApp, RendererGl )







