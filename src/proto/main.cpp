
//#include "CinderApp.h"

#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;

// We'll create a new Cinder Application by deriving from the AppBasic class
class CinderApp : public AppBasic {
  public:
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	void setup();
	void update();
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec2f>		mPoints;
	gl::Texture		myImage;
	// camera
	CameraPersp		mCam;
	Vec3f			mCamLookat;
	Vec3f			mCamPos;
	Vec3f			mCamUp;
	Quatf			mCamRot;

	// Input
	params::InterfaceGl mDebugInterface;
};

void CinderApp::mouseDrag( MouseEvent event )
{
	mPoints.push_back( event.getPos() );
}

void CinderApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void CinderApp::setup()
{
	mCamPos = Vec3f( 0.0f, 0.0f, 500.0f );
	mCamLookat = Vec3f::zero();
	mCamUp = Vec3f::yAxis();
	myImage = gl::Texture( loadImage( loadAsset( "img.jpg" ) ) );

	// Debug interface
	mDebugInterface = params::InterfaceGl( "Gait", Vec2i( 225, 200 ) );
	mDebugInterface.addParam( "Camera angle", &mDebugInterface );
}

void CinderApp::update()
{
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 5.0f, 3000.0f );
	mCam.lookAt( mCamPos, mCamLookat, mCamUp );
	gl::setMatrices( mCam );
}

void CinderApp::draw()
{
	gl::clear( Color( 0.1f, 0.1f, 0.15f ) );

	gl::draw( myImage, getWindowBounds() );

	gl::color( 1.0f, 0.5f, 0.25f );	
	gl::begin( GL_LINE_STRIP );
	for( auto pointIter = mPoints.begin(); pointIter != mPoints.end(); ++pointIter ) {
		gl::vertex( *pointIter );
	}
	gl::end();
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