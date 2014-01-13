
//#include "CinderApp.h"
#include <vld.h>
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
#include "Bone.h"
#include "Foot.h"
#include "IKRig2Joint.h"

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
	void shutdown();
	void update();
	void draw();

	void drawFPS(float& p_baseLine);
	void drawGaitPhaseText(float& p_baseLine);
	void visualizeSkeleton();
	void visualizeGaitCycles(float& p_baseLine);
	void drawSwingEaseGraph(float p_xoffset, float y_offset, float p_width, float p_height);
	void drawBones();

	// windows timer
	__int64 countsPerSec;
	__int64 currTimeStamp;
	double secsPerCount;
	double dt;
	double fps;
	__int64 prevTimeStamp;
	float timeAccumulator;

	// This will maintain a list of points which we will draw line segments between
	gl::Texture		myImage;
	// camera
	CameraPersp		mCam;
	Vec3f			mCamLookat;
	Vec3f			mCamPos;
	Vec3f			mCamUp;
	Quatf			mCamRot;
	float			mCamDist;
	float			mTextBaseLine;

	gl::TextureFontRef*	mFont;
	static const int	mMaxFontSize=20;

	Bone mBone1Test;
	Bone mBone2Test;

	IKRig2Joint* mLeg;
	Foot* mFoot;

	//
	AnimationPlayer mPlayer;

	// Input
	params::InterfaceGlRef mDebugInterface;
};

void CinderApp::mouseDrag( MouseEvent event )
{
// 	Vec2f epos = event.getPos();
// 	Vec3f wpos = Vec3f(((epos.x/getWindowWidth())*2.0f - 1.0f)*mCamDist,-((epos.y/getWindowHeight())*2.0f - 1.0f)*mCamDist,-1.0f);
// 	Matrix44f wmat = mCamRot.toMatrix44();
// 	wpos = wmat.transformPoint(wpos);
// 	mPoints.push_back(wpos);
}

void CinderApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void CinderApp::prepareSettings( Settings *settings )
{
#if defined(_WIN32) && (defined(DEBUG) || defined(_DEBUG))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	SetThreadAffinityMask(GetCurrentThread(), 1);
	countsPerSec=0;
	prevTimeStamp=0;
	currTimeStamp=0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);
	QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
	dt=0.0;
	fps=0.0;
	timeAccumulator=0.0f;
	secsPerCount = (double)(1.0f / (float)countsPerSec);

	settings->setWindowSize( 1280, 768 );
	settings->setFrameRate( 60.0f );
}

void CinderApp::setup()
{
	mFoot = new Foot();
	mLeg = new IKRig2Joint(nullptr,mFoot,2.0f);

	mBone1Test = Bone(nullptr,3.0f);
	mBone2Test = Bone(&mBone1Test,1.0f);

	mTextBaseLine=20.0f;
	mPlayer.init();
	mFont = new gl::TextureFontRef[mMaxFontSize+1];
	for (int i=0;i<=mMaxFontSize;i++)
		mFont[i] = gl::TextureFont::create( Font( "Arial", (float)i+8.0f ));

	// camera
	mCamDist = 10.0f;
	mCamPos = Vec3f( 0.0f, 0.0f, mCamDist );
	mCamLookat = Vec3f::zero();
	mCamUp = Vec3f::yAxis();
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.01f, 1000.0f );

	// funny
	myImage = gl::Texture( loadImage( loadAsset( "img.jpg" ) ) );

	// Debug interface
	mDebugInterface = params::InterfaceGl::create( "Gait", Vec2i( 225, 200 ) );
	mDebugInterface->setOptions("","position='10 300'");
	mDebugInterface->addParam( "Cam angle", &mCamRot, "opened=true");
	mDebugInterface->addParam( "Debug baseline", &mTextBaseLine);
	mDebugInterface->addParam( "Cam dist", &mCamDist, "min=0.02 max=1000.0 step=1.0 keyIncr=s keyDecr=w" );
	mDebugInterface->addParam( "Phase", mPlayer.getGaitPhaseRef() );
	vector<string> easingNames;
	easingNames.push_back("COSINE_INV_NORM");
	easingNames.push_back("HALF_SINE");
	mDebugInterface->addParam( "Easing", easingNames,(int*)mPlayer.getEasingStateRef() );
}


void CinderApp::shutdown()
{
	delete mFoot;
	delete mLeg;
	delete [] mFont;
}


void CinderApp::update()
{
	// Get Delta time	
	prevTimeStamp = currTimeStamp;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
	dt = (currTimeStamp - prevTimeStamp) * secsPerCount;
	dt = min(max(dt,0.0),0.7);
	fps = 1.0/dt;
	timeAccumulator+=(float)dt;

	mPlayer.update((float)dt);

	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 5.0f, 3000.0f );
	mCamPos = Vec3f( 0.0f, 0.0f, mCamDist );
	mCam.lookAt( mCamPos, mCamLookat, mCamUp );

	mFoot->setPosition(Vec3f(0.0f,sin(timeAccumulator),cos(0.5f*timeAccumulator)));
	mLeg->updateRig();
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


	glColor4f( ColorA( 0.0f, 1.0f, 1.0f, 0.1f ) );
	//gl::drawColorCube( mCamLookat, Vec3f(10.0f,10.0f,10.0f) );
	gl::drawCoordinateFrame();

	visualizeSkeleton();
	drawBones();

	glColor4f( ColorA( 1.0f, 0.0f, 0.5f, 1.0f ) );
	gl::disableDepthRead();
	gl::disableDepthWrite();
	float baseLine = mTextBaseLine;
	gl::setMatricesWindow(getWindowWidth(),getWindowHeight());
	drawFPS(baseLine);
	drawGaitPhaseText(baseLine);
	visualizeGaitCycles(baseLine);
	mDebugInterface->draw();
}

void CinderApp::visualizeGaitCycles(float& p_baseLine)
{
	GaitCycle* cycles = mPlayer.getGaitDataRef();

	float currentT=*mPlayer.getGaitPhaseRef();
	float timelineLen=100.0f;
	float timelineFeetOffset=10.0f;
	float xpad=30.0f+timelineLen+timelineFeetOffset;
	float feetW=10.0f;
	float feetH=10.0f;
	float feetMargin=15.0f;
	float bodyW=50.0f;
	float bodyH=(float)(cycles->mFeetCount-1)*(feetH+feetMargin)+feetMargin;
	// Draw body visualization		
	p_baseLine+=feetH;
	gl::drawStrokedRect(Rectf(xpad,p_baseLine,xpad+bodyW,p_baseLine+bodyH)); // body
	gl::drawStrokedRect(Rectf(xpad+(bodyW-feetW)*0.5f,p_baseLine,xpad+(bodyW+feetW)*0.5f,p_baseLine-feetH)); // head
	p_baseLine+=feetMargin;

	int fsize=5;

	// Draw feet visualization
	for (int i=0;i<cycles->mFeetCount;i+=2)
	{
		glColor4f( ColorA( 1.0f, 0.0f, 0.5f, 1.0f ) );
		float y1=p_baseLine+(feetMargin+feetH)*i;
		float y2=y1+feetH;
		StepCycle* cycleL = &cycles->mStepCycles[i];
		StepCycle* cycleR = &cycles->mStepCycles[i+1];
		float offsetL=cycleL->mNormStepTrigger;
		float offsetR=cycleR->mNormStepTrigger;
		float lenL=cycleL->mNormDutyFactor;
		float lenR=cycleR->mNormDutyFactor;

		// feet
		// left
		Rectf foot = Rectf(xpad-feetW,y1,xpad,y2);
		bool leftHit=false;
		bool rightHit=false;
		if (cycleL->isInStance(currentT))
		{
			gl::drawSolidRect(foot);
			leftHit=true;
		}
		else
			gl::drawStrokedRect(foot);
		// right
		foot = Rectf(xpad+bodyW,y1,xpad+bodyW+feetW,y2);
		if (cycleR->isInStance(currentT))
		{
			gl::drawSolidRect(foot);
			rightHit=true;
		}
		else
			gl::drawStrokedRect(foot);
		// timelines
		// left
		float lineStart=xpad-feetW-timelineLen-timelineFeetOffset;
		float lineEnd=xpad-feetW-timelineFeetOffset;
		float dutyEnd = lineStart+timelineLen*(offsetL+lenL);
		glColor4f( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) ); // back
		gl::drawLine(Vec2f(lineStart,y1),Vec2f(lineEnd,y1));
		if (!leftHit)
			glColor4f( ColorA( 1.0f, 0.0f, 0.5f, 1.0f ) ); // front (contact time)
		else
			glColor4f( ColorA( 1.0f, 0.2f, 0.7f, 1.0f ) );
		gl::drawLine(Vec2f(lineStart+timelineLen*offsetL,y1),Vec2f(min(lineEnd,dutyEnd),y1));
		if (offsetL+lenL>1.0f) // draw rest if out of bounds
			gl::drawLine(Vec2f(lineStart,y1),Vec2f(lineStart+timelineLen*(offsetL+lenL-1.0f),y1));

		// draw duty factor
		mFont[fsize]->drawString(string("DF: ")+toString(cycleL->mNormDutyFactor),
			Vec2f(lineStart+timelineLen*min(offsetL+lenL*0.5f,1.0f),y1+5+fsize));

		// draw swing phase
		glColor4f( ColorA( 0.8f, 1.0f, 0.1f, 1.0f ) );
		mFont[fsize]->drawString(string("swing: ")+toString(cycleL->getSwingPhase(currentT),4),
			Vec2f(lineStart+timelineLen*min(offsetL+lenL*0.5f,1.0f),y1+15+fsize));

		// draw step trigger
		glColor4f( ColorA( 0.5f, 1.0f, 1.0f, 1.0f ) );
		mFont[fsize]->drawString(toString(cycleL->mNormStepTrigger),
			Vec2f(lineStart+timelineLen*offsetL,y1-(5+fsize)));
		
		// Draw current time marker
		glColor4f( ColorA( 0.0f, 1.0f, 0.5f, 1.0f ) );
		gl::drawLine(Vec2f(lineStart+timelineLen*currentT,y1-1.0f),Vec2f(lineStart+timelineLen*currentT,y1+1.0f));


		// right
		lineStart=xpad+bodyW+feetW+timelineFeetOffset;
		lineEnd=lineStart+timelineLen;
		dutyEnd = lineStart+timelineLen*(offsetR+lenR);
		glColor4f( ColorA( 0.0f, 0.0f, 0.0f, 1.0f ) ); // back
		gl::drawLine(Vec2f(lineStart,y1),Vec2f(lineEnd,y1));
		if (!rightHit)
			glColor4f( ColorA( 1.0f, 0.0f, 0.5f, 1.0f ) ); // front (contact time)
		else
			glColor4f( ColorA( 1.0f, 0.2f, 0.7f, 1.0f ) );
		gl::drawLine(Vec2f(lineStart+timelineLen*offsetR,y1),Vec2f(min(lineEnd,dutyEnd),y1));
		if (offsetR+lenR>1.0f)
			gl::drawLine(Vec2f(lineStart,y1),Vec2f(lineStart+timelineLen*(offsetR+lenR-1.0f),y1));

		// draw duty factor
		mFont[fsize]->drawString(string("DF: ")+toString(cycleR->mNormDutyFactor),
			Vec2f(lineStart+timelineLen*min(offsetR+lenR*0.5f,1.0f),y1+5+fsize));

		// draw swing phase
		glColor4f( ColorA( 0.8f, 1.0f, 0.1f, 1.0f ) );
		mFont[fsize]->drawString(string("swing: ")+toString(cycleR->getSwingPhase(currentT),4),
			Vec2f(lineStart+timelineLen*min(offsetR+lenR*0.5f,1.0f),y1+15+fsize));

		// draw step trigger
		glColor4f( ColorA( 0.5f, 1.0f, 1.0f, 1.0f ) );
		mFont[fsize]->drawString(toString(cycleR->mNormStepTrigger),
			Vec2f(lineStart+timelineLen*offsetR,y1-(5+fsize)));

		// Draw current time marker
		glColor4f( ColorA( 0.0f, 1.0f, 0.5f, 1.0f ) );
		gl::drawLine(Vec2f(lineStart+timelineLen*currentT,y1-1.0f),Vec2f(lineStart+timelineLen*currentT,y1+1.0f));
	}

	p_baseLine+=bodyH+5.0f;

	// Draw ease function
	drawSwingEaseGraph( xpad, p_baseLine, bodyW, feetH*2.0f );

	p_baseLine+=feetH*2.0f;
}

void CinderApp::drawGaitPhaseText( float& p_baseLine )
{
	int fsize=15;
	mFont[fsize]->drawString(toString(*mPlayer.getGaitPhaseRef()),
		Vec2f(2.0f,p_baseLine));
	p_baseLine+=(float)fsize;
}

void CinderApp::drawFPS( float& p_baseLine )
{
	int fsize=10;
	mFont[fsize]->drawString(toString((int)fps),
		Vec2f(2.0f,p_baseLine));
	p_baseLine+=(float)fsize;
}

void CinderApp::drawBones()
{

	// IK
	glColor4f( ColorA( 1.0f, 0.0f, 0.0f, 1.0f ) );
	Vec3f start=mLeg->getUpperBone()->getOrigin();
	Vec3f end = mLeg->getUpperBone()->getEnd();
	gl::drawVector(start,end);

	glColor4f( ColorA( 1.0f, 0.0f, 1.0f, 1.0f ) );
	start=mLeg->getLowerBone()->getOrigin();
	end = mLeg->getLowerBone()->getEnd();
	gl::drawVector(start,end);

	gl::drawColorCube(mFoot->getPosition(),Vec3f(0.1f,0.1f,0.2f));
}

void CinderApp::visualizeSkeleton()
{
	GaitCycle* cycles = mPlayer.getGaitDataRef();

	float currentT=*mPlayer.getGaitPhaseRef();
	float heightFromGround=1.0f;
	float legMargin=1.0f;
	float bodyW=1.0f;
	float bodyH=1.0f;
	float jointSz=0.3f;
	float bodyL=(float)(cycles->mFeetCount-1)*legMargin+legMargin;

	gl::drawColorCube(Vec3f(0.0f,heightFromGround,0.0f),Vec3f(bodyW,bodyH,bodyL));
	gl::drawColorCube(Vec3f(0.0f,heightFromGround,bodyL*0.5f),Vec3f(0.3f,0.3f,0.3f));

	// Draw leg+feet visualization
	for (int i=0;i<cycles->mFeetCount;i+=2)
	{
		StepCycle* cycleL = &cycles->mStepCycles[i];
		StepCycle* cycleR = &cycles->mStepCycles[i+1];

		// left
		gl::drawColorCube(Vec3f(bodyW*0.5f,heightFromGround-bodyH*0.5f,bodyL*0.5f-legMargin-legMargin*(float)i),
			Vec3f(jointSz,jointSz,jointSz));
		if (cycleL->isInStance(currentT))
		{
			gl::drawCube(Vec3f(bodyW*0.5f,heightFromGround-bodyH*0.5f,bodyL*0.5f-legMargin-legMargin*(float)i),
				Vec3f(jointSz,jointSz,jointSz)*2.0f);
		}

		// right
		gl::drawColorCube(Vec3f(bodyW*-0.5f,heightFromGround-bodyH*0.5f,bodyL*0.5f-legMargin-legMargin*(float)i),
			Vec3f(jointSz,jointSz,jointSz));
		if (cycleR->isInStance(currentT))
		{
			gl::drawCube(Vec3f(bodyW*-0.5f,heightFromGround-bodyH*0.5f,bodyL*0.5f-legMargin-legMargin*(float)i),
				Vec3f(jointSz,jointSz,jointSz)*2.0f);
		}
	}
}

void CinderApp::drawSwingEaseGraph( float p_xoffset, float y_offset, float p_width, float p_height )
{
	// draw ease function
	gl::color( 1.0f, 0.5f, 0.25f );  
	gl::begin( GL_LINE_STRIP );
	int numpoints=20;
	p_height*=0.5f;
	for( int i=0; i<=numpoints; i++ ) 
	{
		float e_t=((float)i/(float)numpoints);
		gl::vertex(p_xoffset+e_t*p_width,y_offset+p_height-mPlayer.autoEase(e_t)*p_height);
	}
	gl::end();
	gl::color( 0.7f, 0.5f, 0.25f ); 
	gl::drawLine(Vec2f(p_xoffset,y_offset+p_height),Vec2f(p_xoffset+p_width,y_offset+p_height));
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