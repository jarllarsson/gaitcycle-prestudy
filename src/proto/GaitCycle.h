#pragma once

#include "StepCycle.h"

// =======================================================================================
//                                      GaitCycle
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Gait cycle data for a collection of feet
///        
/// # GaitCycle
/// Detailed description.....
/// Created on: 27-12-2013 
///---------------------------------------------------------------------------------------

struct GaitCycle
{
public:
	virtual ~GaitCycle() {delete [] mStepCycles;}
	/// 
	/// Number of feet (divisible by 2).
	///
	int			mFeetCount;
	/// 
	/// Collection of normalized step cycles, one for
	/// the movement of each foot.
	///
	StepCycle* mStepCycles;
	///
	/// The time it takes to complete the gait cycle
	///
	float		mGaitPeriod;
};