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
	enum JOINT_TYPE
	{
		PLANTIGRADE,
		DIGITIGRADE_FRONT_APPROX
	};
	IKRig2Joint(Bone* p_parent, Foot* p_foot, float p_len, JOINT_TYPE p_joint);
	virtual ~IKRig2Joint();
	Bone* getUpperBone();
	Bone* getLowerBone();
	void updateRig();
	JOINT_TYPE* getJointTypeRef();
	void setJointType(JOINT_TYPE p_jointType);

protected:
private:
	JOINT_TYPE mJointType; // bird approx (they don't really have flipped knees, but this is a simple way of faking it)
	Foot* mFoot;
	Bone* mUpperBone;
	Bone* mLowerBone;
};