#pragma once
#include "Foot.h"
#include "Bone.h"

// =======================================================================================
//                                      IKRig2Joint
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	A super simple IK rig
///        
/// # IKRig2Joint
/// Detailed description.....
/// Created on: 30-12-2013 
///---------------------------------------------------------------------------------------

class IKRig2Joint
{
public:
	IKRig2Joint(Bone* p_parent, Foot* p_foot, float p_len);
	virtual ~IKRig2Joint();
	Bone* getUpperBone();
	Bone* getLowerBone();
	void updateRig();

protected:
private:
	Foot* mFoot;
	Bone* mUpperBone;
	Bone* mLowerBone;
};