#include "Foot.h"

Foot::Foot()
{
	mOldPos = mPos = Vec3f(0.0f,0.0f,0.0f);
	mStanceEnd=mStanceStart=Vec3f(0.0f,0.0f,0.0f);
	mIsInStance=false;
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

void Foot::updateSpeed( PHASE_STATUS p_status, float p_dt )
{
	if (p_status==PHASE_STATUS::STANCE)
	{
		if (mIsInStance==false)
		{
			mStanceStart=mPos+Vec3f(0.0f,0.05f,0.0f);
		}
		mSpeed=(mPos.z-mOldPos.z)/p_dt;
		mSpeed*=-1.0f;
		mStanceEnd=mPos;
		mStanceEnd.y = mStanceStart.y;
		mIsInStance=true;
	}
	else
	{
		mSpeed=0.0f;
		mIsInStance=false;
	}
	mOldPos=mPos;
}

float Foot::getSpeed()
{
	return mSpeed;
}

Vec3f& Foot::getStanceStart()
{
	return mStanceStart;
}

Vec3f& Foot::getStanceEnd()
{
	return mStanceEnd;
}

bool Foot::isInStance()
{
	return mIsInStance;
}

Vec3f& Foot::getOldPosition()
{
	return mOldPos;
}
