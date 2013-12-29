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
		return p_t>mNormStepTrigger&&p_t<mNormStepTrigger+mNormDutyFactor;
	}
};