
//#include "CinderApp.h"

#include "cinder/app/AppBasic.h"
#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;

// We'll create a new Cinder Application by deriving from the AppBasic class
class CinderApp : public AppBasic {
  public:
	void mouseDrag( MouseEvent event );
	void keyDown( KeyEvent event );
	void draw();

	// This will maintain a list of points which we will draw line segments between
	list<Vec2f>		mPoints;
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

void CinderApp::draw()
{
	gl::clear( Color( 0.1f, 0.1f, 0.15f ) );

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