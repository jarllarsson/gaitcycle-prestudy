
//#include "CinderApp.h"

#include "AnimationPlayer.h"

#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include <list>
#include <string>
#include "ToString.h"
#include <cinder/gl/TextureFont.h>

using namespace ci;
using namespace ci::app;
using namespace std;

// We'll create a new Cinder Application by deriving from the AppBasic class
class CinderApp : public AppBasic {
  public:
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec3f>		mPoints;
	gl::Texture		myImage;
	// camera
	CameraPersp		mCam;
	Vec3f			mCamLookat;
	Vec3f			mCamPos;
	Vec3f			mCamUp;
	Quatf			mCamRot;
	float			mCamDist;
	float mTextBaseLine;

	gl::TextureFontRef	mFont;

	//
	AnimationPlayer mPlayer;

	// Input
	params::InterfaceGlRef mDebugInterface;
};

void CinderApp::mouseDrag( MouseEvent event )
{
	Vec2f epos = event.getPos();
	Vec3f wpos = Vec3f(((epos.x/getWindowWidth())*2.0f - 1.0f)*mCamDist,-((epos.y/getWindowHeight())*2.0f - 1.0f)*mCamDist,-1.0f);
	Matrix44f wmat = mCamRot.toMatrix44();
	wpos = wmat.transformPoint(wpos);
	mPoints.push_back(wpos);
}

void CinderApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void CinderApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
}

void CinderApp::setup()
{
	mTextBaseLine=20.0f;
	mPlayer.init();
	mFont = gl::TextureFont::create( Font( "Arial", 30 ));

	// camera
	mCamDist = 500.0f;
	mCamPos = Vec3f( 0.0f, 0.0f, mCamDist );
	mCamLookat = Vec3f::zero();
	mCamUp = Vec3f::yAxis();
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 5.0f, 3000.0f );

	// funny
	myImage = gl::Texture( loadImage( loadAsset( "img.jpg" ) ) );

	// Debug interface
	mDebugInterface = params::InterfaceGl::create( "Gait", Vec2i( 225, 200 ) );
	mDebugInterface->addParam( "Cam angle", &mCamRot);
	mDebugInterface->addParam( "txt", &mTextBaseLine);
	mDebugInterface->addParam( "Cam dist", &mCamDist, "min=50.0 max=1000.0 step=50.0 keyIncr=s keyDecr=w" );
	mDebugInterface->addParam( "Phase", mPlayer.getGaitPhaseRef() );
}

void CinderApp::update()
{
	mPlayer.update(1.0f/60.0f);

	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 5.0f, 3000.0f );
	mCamPos = Vec3f( 0.0f, 0.0f, mCamDist );
	mCam.lookAt( mCamPos, mCamLookat, mCamUp );
}

void CinderApp::draw()
{	
	gl::clear( Color( 0.1f, 0.1f, 0.15f ), true );
	gl::color( 1.0f, 1.0f, 1.0f );
	gl::draw( myImage, getWindowBounds() );


	gl::setMatrices( mCam );
	gl::rotate( mCamRot );
	//

	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::enableAlphaBlending();


	gl::color( 1.0f, 1.0f, 1.0f );	

	gl::color( 1.0f, 0.5f, 0.25f );	
	gl::begin( GL_LINE_STRIP );
	for( auto pointIter = mPoints.begin(); pointIter != mPoints.end(); ++pointIter ) {
		gl::vertex( *pointIter );
	}

	gl::end();
	glColor4f( ColorA( 0.0f, 1.0f, 1.0f, 0.1f ) );
	gl::drawColorCube( mCamLookat, Vec3f(10.0f,10.0f,10.0f) );

	glColor4f( ColorA( 1.0f, 0.0f, 0.5f, 1.0f ) );
	gl::disableDepthRead();
	gl::disableDepthWrite();
	gl::setMatricesWindow(getWindowWidth(),getWindowHeight());
	mFont->drawString(toString(*mPlayer.getGaitPhaseRef()),
		Vec2f(2.0f,mTextBaseLine));
	mDebugInterface->draw();
}


// This line tells Cinder to actually create the application
CINDER_APP_BASIC( CinderApp, RendererGl )
/*
int main()
{
	

	cout<<"Hello World!";
	return 0;
}
*/