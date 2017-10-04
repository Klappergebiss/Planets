#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/GeomIo.h"
#include "cinder/Rand.h"
//#include "cinder/gl/Fbo.h"  //for blur
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
    
    PlanetManager mPlanetManager;
//    gl::FboRef mFbo;    //for blur
//    int test;
};

void PlanetsApp::setup()
{
//    test = 10;
    time_t currentTime;
    time(&currentTime);
    
    randSeed((double)currentTime);
    setWindowPos(0, 0);
    setWindowSize(getDisplay()->getWidth(), getDisplay()->getHeight());
    setFullScreen(true);
    
    mPlanetManager.addStars(135);
    
    mPlanetManager.addPlanets(17);  //5
    hideCursor();
    
//    mFbo->create(getDisplay()->getWidth(), getDisplay()->getHeight());    //for blur
}


void PlanetsApp::update()
{
    mPlanetManager.update();
    
}

/*
void PlanetsApp::draw() {   //test for blur
    mFbo->bindFramebuffer(); // mFbo is your frame buffer object (ci::gl::Fbo)
    gl::enableAlphaBlending();
    gl::color( ColorAf( 0.0f, 0.0f, 0.0f, 0.5f ) );
    gl::drawSolidRect( mFbo->getBounds() ); // Dim last frame
    // Draw new stuff here
    
    mPlanetManager.draw();
    mFbo->unbindFramebuffer();
    
    gl::color( ColorAf::black() );
//    mShader.bind(); // Gaussian blur shader
//    gl::draw( mFbo->getTexture() );
//    mShader.unbind();
}
*/

void PlanetsApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );  //not if test
    //
    // if (test != 0) {
    //    gl::clear( Color( 0, 0, 0 ) );
    //    test--;
    // }
    //
    mPlanetManager.draw();
}

void PlanetsApp::keyDown( KeyEvent event) {
    if (event.getCode() == KeyEvent::KEY_ESCAPE) {
        setFullScreen(false);
        showCursor();
    }
    if (event.getCode() == 'c') mPlanetManager.enableCollide = !mPlanetManager.enableCollide;
}

CINDER_APP( PlanetsApp, RendererGl )







