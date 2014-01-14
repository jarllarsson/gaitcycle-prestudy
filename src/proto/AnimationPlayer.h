#pragma once

#include "GaitCycle.h"

// =======================================================================================
//                                      AnimationPlayer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handler for playing gait data
///        
/// # AnimationPlayer
/// Detailed description.....
/// Created on: 27-12-2013 
///---------------------------------------------------------------------------------------

class AnimationPlayer
{
public:
	enum EASING_STATE
	{
		COSINE_INV_NORM,
		HALF_SINE,
	};
	AnimationPlayer();
	virtual ~AnimationPlayer();

	void init(int p_feetCount=4);
	void update(float p_dt);
	GaitCycle* getGaitDataRef() const;
	float* getGaitPhaseRef();
	EASING_STATE* getEasingStateRef();
	void setEasingState(EASING_STATE p_state);

	void setStrideLength(float p_value);
	void setSwingHeight(float p_value);
	float getStrideLength();
	float getSwingHeight();
	float* getStrideLengthRef();
	float* getSwingHeightRef();

	float autoEase(float p_t);
	float easeCosineInvNorm(float p_t);
	float easeHalfSine(float p_t);
protected:
private:
	GaitCycle*	mGaitCycle;
	EASING_STATE m_easing;
	float		mGaitPhase; ///< The current phase 0..1
	float		mStrideLength;
	float		mSwingHeight;
};