#pragma once

// =======================================================================================
//                                      StepCycle
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Data describing movement for one foot in normalized time
///        
/// # StepCycle
/// Data describing movement for one foot in normalized time in relation to the overall 
/// gait of all limbs.
/// Created on: 27-12-2013 
///---------------------------------------------------------------------------------------

class StepCycle
{
public:
	///
	/// mNormDutyFactor
	/// Fraction of overall normalized time for which the 
	/// foot is touching the ground.
	///
	float mNormDutyFactor; 

	///
	/// mNormStepTrigger
	/// Offset time point in normalized time when the
	/// foot begins its cycle.
	///
	float mNormStepTrigger;

	bool isInStance(float p_t)
	{
		// p_t is always < 1
		float max = mNormStepTrigger+mNormDutyFactor;
		return (max<=1.0f && p_t>=mNormStepTrigger && p_t<max) || // if within bounds, if more than offset and less than offset+len
			   (max>1.0f && ((p_t>=mNormStepTrigger) || p_t<max-1.0f) ); // if phase shifted out of bounds(>1), more than offset or less than len-1
	}
};