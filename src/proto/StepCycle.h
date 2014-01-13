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
		float maxt = mNormStepTrigger+mNormDutyFactor;
		return (maxt<=1.0f && p_t>=mNormStepTrigger && p_t<maxt) || // if within bounds, if more than offset and less than offset+len
			   (maxt>1.0f && ((p_t>=mNormStepTrigger) || p_t<maxt-1.0f) ); // if phase shifted out of bounds(>1), more than offset or less than len-1
	}

	//
	// Get phase of swing, is zero in stance (0-1-0)
	// can be used as period in transition function
	//
	float getSwingPhase(float p_t)
	{
		// p_t is always < 1
		if (isInStance(p_t)) return 0.0f;
		float maxt = mNormStepTrigger+mNormDutyFactor;
		float pos=p_t;
		float swinglen = 1.0f-mNormDutyFactor; // get total swing time
		if (maxt<=1.0f) // max is inside bounds
		{
			float rest = 1.0f-maxt; // rightmost rest swing			
			if (p_t>maxt) 
				pos-=maxt; // get start as after end of stance		
			else
				pos+=rest; // add rest when at beginning again
			pos /= swinglen; // normalize
			//pos= 1.0f-pos; // invert
		}
		else // max is outside bounds
		{
			float mint=maxt-1.0f; // start
			pos -= mint;
			pos /= swinglen;
		}
		return pos;
	}
};