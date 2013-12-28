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
	AnimationPlayer();
	virtual ~AnimationPlayer();

	void init();
	void update(float p_dt);
	GaitCycle* getGaitDataRef() const;
	float* getGaitPhaseRef();
protected:
private:
	GaitCycle*	mGaitCycle;

	float		mGaitPhase; ///< The current phase 0..1
	float		mStrideLength;
	float		mSwingHeight;
};