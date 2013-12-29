
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

	void drawGaitPhaseText(float& p_baseLine);
	void visualizeGaitCycles(float& p_baseLine);

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

	gl::TextureFontRef*	mFont;
	static const int	mMaxFontSize=20;

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
	mFont = new gl::TextureFontRef[mMaxFontSize+1];
	for (int i=0;i<=mMaxFontSize;i++)
		mFont[i] = gl::TextureFont::create( Font( "Arial", i+5 ));

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
	mDebugInterface->setOptions("GLOBAL","Gait position='10 300'");
	mDebugInterface->addParam( "Cam angle", &mCamRot, "visible=true");
	mDebugInterface->addParam( "Debug baseline", &mTextBaseLine);
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
	float baseLine = mTextBaseLine;
	gl::setMatricesWindow(getWindowWidth(),getWindowHeight());
	drawGaitPhaseText(baseLine);
	visualizeGaitCycles(baseLine);
	mDebugInterface->draw();
}

void CinderApp::visualizeGaitCycles(float& p_baseLine)
{
	GaitCycle* cycles = mPlayer.getGaitDataRef();
	// Draw body visualization
	float timelineLen=100.0f;
	float timelineFeetOffset=10.0f;
	float xpad=30.0f+timelineLen+timelineFeetOffset;
	float feetW=10.0f;
	float feetH=10.0f;
	float feetMargin=10.0f;
	p_baseLine+=feetMargin;
	float bodyW=50.0f;
	float bodyH=(float)(cycles->mFeetCount)*(feetH+feetMargin);
	gl::drawStrokedRect(Rectf(xpad,p_baseLine,xpad+bodyW,p_baseLine+bodyH));
	
	int fsize=10;

	// Draw feet visualization
	for (int i=0;i<cycles->mFeetCount;i+=2)
	{
		glColor4f( ColorA( 1.0f, 0.0f, 0.5f, 1.0f ) );
		float y1=p_baseLine+(feetMargin+feetH)*i;
		float y2=y1+feetH;
		float offsetL=cycles->mStepCycles[i].mNormStepTrigger;
		float offsetR=cycles->mStepCycles[i+1].mNormStepTrigger;
		float lenL=cycles->mStepCycles[i].mNormDutyFactor;
		float lenR=cycles->mStepCycles[i+1].mNormDutyFactor;
		// feet
		// left
		gl::drawStrokedRect(Rectf(xpad-feetW,y1,xpad,y2));
		// right
		gl::drawStrokedRect(Rectf(xpad+bodyW,y1,xpad+bodyW+feetW,y2));
		// timelines
		// left
		float lineStart=xpad-feetW-timelineLen-timelineFeetOffset;
		float lineEnd=xpad-feetW-timelineFeetOffset;
		glColor4f( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) ); // back
		gl::drawLine(Vec2f(lineStart,y1),Vec2f(lineEnd,y1));
		glColor4f( ColorA( 1.0f, 0.0f, 0.5f, 1.0f ) ); // front (contact time)
		gl::drawLine(Vec2f(lineStart+timelineLen*offsetL,y1),Vec2f(lineStart+timelineLen*(offsetL+lenL),y1));
		// right
		lineStart=xpad+bodyW+feetW+timelineFeetOffset;
		lineEnd=lineStart+timelineLen+timelineFeetOffset;
		glColor4f( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) ); // back
		gl::drawLine(Vec2f(lineStart,y1),Vec2f(lineEnd,y1));
		glColor4f( ColorA( 1.0f, 0.0f, 0.5f, 1.0f ) ); // front (contact time)
		gl::drawLine(Vec2f(lineStart+timelineLen*offsetR,y1),Vec2f(lineStart+timelineLen*(offsetR+lenR),y1));
	}

	p_baseLine+=bodyH+5.0f;
}

void CinderApp::drawGaitPhaseText( float& p_baseLine )
{
	int fsize=10;
	mFont[fsize]->drawString(toString(*mPlayer.getGaitPhaseRef()),
		Vec2f(2.0f,p_baseLine));
	p_baseLine+=(float)fsize;
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