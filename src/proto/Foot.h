#pragma once
#include <cinder\Vector.h>
using namespace ci;
// =======================================================================================
//                                      Foot
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A foot
///        
/// # Foot
/// Detailed description.....
/// Created on: 27-12-2013 
///---------------------------------------------------------------------------------------

class Foot
{
public:
	enum PHASE_STATUS
	{
		SWING,
		STANCE
	};

	Foot();
	virtual ~Foot();

	Vec3f& getPosition();
	Vec3f& getOldPosition();
	void setPosition(Vec3f& p_pos);
	void updateSpeed(PHASE_STATUS p_status, float p_dt);
	float getSpeed();
	Vec3f& getStanceStart();
	Vec3f& getStanceEnd();
	bool isInStance();
protected:
private:
	bool mIsInStance;
	Vec3f mStanceStart;
	Vec3f mStanceEnd;
	float mSpeed;
	Vec3f mPos;
	Vec3f mOldPos;
};