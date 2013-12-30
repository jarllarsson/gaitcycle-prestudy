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
	Foot();
	virtual ~Foot();

	Vec3f& getPosition();
	void setPosition(Vec3f& p_pos);
protected:
private:
	Vec3f mPos;
};