#include "Bone.h"

Bone::Bone( Bone* p_parent/*=nullptr*/, float p_length/*=1.0f*/ )
{
	mParent=p_parent;
	mLength=p_length;
	mPoseDir=Vec3f(0.0f,-1.0f,0.0f);
	mTransform=Matrix44<float>::identity();
	reset();
	applyHierarchicalTransform();
}

Bone::~Bone()
{

}

Vec3f& Bone::getPoseDirRef()
{
	return mPoseDir;
}

float Bone::getLength()
{
	return mLength;
}

Matrix44<float>& Bone::getTransformRef()
{
	return mTransform;
}

Bone* Bone::getParent()
{
	return mParent;
}

void Bone::reset()
{
	Vec3f end=mPoseDir*mLength;
	mTransform = Matrix44<float>::createTranslation(end);
}


void Bone::setRotation( float p_angle )
{
	reset();
	// Hardcoded as 1-DOF, on X-axis
	Vec3f end1 = getEnd();
	mTransform.rotate(Vec3f(1.0f,0.0f,0.0f),p_angle);
	Vec3f end2 = getEnd();
	int i=0;
}

void Bone::applyHierarchicalTransform()
{
	if (mParent!=nullptr)
	{
		mTransform*=mParent->getTransformRef();		
	}
}

Vec3f Bone::getOrigin()
{
	if (mParent!=nullptr)
	{
		Vec3f orig = mParent->getEnd();
		return orig;
	}
	else
	{
		return Vec3f(0.0f,0.0f,0.0f);
	}
}
Vec3f Bone::getEnd()
{
	Vec3f end = getOrigin()+mTransform.transformVec(mPoseDir*mLength);
	return end;
}

void Bone::setLength( float p_len )
{
	mLength=p_len;
}


