#include "Foot.h"

Foot::Foot()
{
	mPos = Vec3f(0.0f,0.0f,0.0f);
}

Foot::~Foot()
{

}

Vec3f& Foot::getPosition()
{
	return mPos;
}

void Foot::setPosition( Vec3f& p_pos )
{
	mPos=p_pos;
}
