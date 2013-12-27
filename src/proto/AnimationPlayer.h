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
protected:
private:
	GaitCycle mGaitCycle;
	///
	/// The current phase 0..1
	///
	float		mGaitPhase;
};