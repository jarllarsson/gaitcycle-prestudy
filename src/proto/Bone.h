#pragma once

#include <cinder/Matrix.h>
#include <cinder/Vector.h>
#include <cinder/Matrix44.h>
#include <cinder/Quaternion.h>
using namespace ci;

// =======================================================================================
//                                      Bone
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Structure for 1-DOF skeleton bone
///        
/// # Bone
/// Detailed description.....
/// Created on: 30-12-2013 
///---------------------------------------------------------------------------------------

class Bone
{
public:
	Bone(Bone* p_parent=nullptr, float p_length=1.0f);
	virtual ~Bone();
	Vec3f& getPoseDirRef();
	float getLength();
	float* getLengthRef();
	Matrix44<float>& getTransformRef();
	Bone* getParent();
	void reset();
	void setRotation(float p_angle);
	void applyHierarchicalTransform();
	Vec3f getOrigin();
	Vec3f getEnd();

	void setLength(float p_len);
protected:
private:
	Vec3f mPoseDir;
	float mLength;
	Bone* mParent;
	Matrix44<float> mTransform;
};