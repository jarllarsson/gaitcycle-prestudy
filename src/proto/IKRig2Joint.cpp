#include "IKRig2Joint.h"

IKRig2Joint::IKRig2Joint( Bone* p_parent, Foot* p_foot, float p_len, JOINT_TYPE p_joint )
{
	mUpperBone=new Bone(p_parent,p_len*0.5f);
	mLowerBone=new Bone(mUpperBone,p_len*0.5f);
	mFoot=p_foot;
	mJointType=p_joint;
}

IKRig2Joint::~IKRig2Joint()
{
	delete mLowerBone;
	delete mUpperBone;
}

Bone* IKRig2Joint::getUpperBone()
{
	return mUpperBone;
}

Bone* IKRig2Joint::getLowerBone()
{
	return mLowerBone;
}

void IKRig2Joint::updateRig()
{
	int kneeFlip=1;
	if (mJointType==JOINT_TYPE::BIRD_FAKE_APPROX)
		kneeFlip=-1;
	Vec3f footPos=mFoot->getPosition();
	Vec3f topToFoot = footPos-mUpperBone->getOrigin();
	float toFootLen = topToFoot.length();
	float upperLegAngle=0.0f;
	float lowerLegAngle=0.0f;
	float uB = mUpperBone->getLength();
	float lB = mLowerBone->getLength();	
	// first get offset angle beetween foot and axis
	float offsetAngle = atan2(topToFoot.y,-topToFoot.z);
	// If dist to foot is shorter than combined leg length
	bool straightLeg=false;
	if (toFootLen<uB+lB)
	{
		float uBS = uB*uB;
		float lBS = lB*lB;
		float hBS = toFootLen*toFootLen;
		// law of cosines for first angle
		upperLegAngle = (-1*kneeFlip)*acos((hBS + uBS - lBS)/(2.0f*uB*toFootLen))+offsetAngle;
		// second angle
		Vec2f newLeg = Vec2f(uB*cos(upperLegAngle),uB*sin(upperLegAngle));
		lowerLegAngle = atan2(topToFoot.y-newLeg.y,-topToFoot.z-newLeg.x)-upperLegAngle;
		/*lowerLegAngle = acos((uBS + lBS - hBS)/(2.0f*uB*lB))-upperLegAngle;*/
	}
	else // otherwise, straight leg
	{
		upperLegAngle=offsetAngle;
		lowerLegAngle=0.0f;
		straightLeg=true;
	}
	
	// update bones
	mUpperBone->setRotation(upperLegAngle);
	mUpperBone->applyHierarchicalTransform();
	mLowerBone->setRotation(lowerLegAngle);
	mLowerBone->applyHierarchicalTransform();

	if (straightLeg)
		mFoot->setPosition(mLowerBone->getEnd());
}

IKRig2Joint::JOINT_TYPE* IKRig2Joint::getJointTypeRef()
{
	return &mJointType;
}

void IKRig2Joint::setJointType( JOINT_TYPE p_jointType )
{
	mJointType=p_jointType;
}
