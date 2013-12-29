#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer()
{
	mGaitCycle = new GaitCycle;
	mGaitPhase=0.0f;
	mStrideLength=1.0f;
	mSwingHeight=1.0f;
}

AnimationPlayer::~AnimationPlayer()
{
	delete mGaitCycle;
}

void AnimationPlayer::init()
{
	if (mGaitCycle)
	{
		int n=4;
		mGaitCycle->mFeetCount = n;
		mGaitCycle->mGaitPeriod = 5.0f;
		mGaitCycle->mStepCycles = new StepCycle[n];
		// Fill with appropriate data
		float c=0.25f;
		for (int i=0;i<n;i++)
		{		
			StepCycle* stepCycle = &(mGaitCycle->mStepCycles[i]);
			stepCycle->mNormDutyFactor = 0.5f;
			stepCycle->mNormStepTrigger = c; // horse walk
			// safety clamp
			/*while (stepCycle->mNormStepTrigger+stepCycle->mNormDutyFactor > 1.0f)
				stepCycle->mNormStepTrigger += 1.0f-(stepCycle->mNormStepTrigger+stepCycle->mNormDutyFactor);*/
			if (stepCycle->mNormStepTrigger > 1.0f)
			{
				stepCycle->mNormStepTrigger=0.0f;
				c=0.0f;
			}
			c+=0.5f;
		}

	}
}

void AnimationPlayer::update( float p_dt )
{
	mGaitPhase += p_dt/mGaitCycle->mGaitPeriod;
	while (mGaitPhase>1.0f)
		mGaitPhase-=1.0f;
}

GaitCycle* AnimationPlayer::getGaitDataRef() const
{
	return mGaitCycle;
}

float* AnimationPlayer::getGaitPhaseRef()
{
	return &mGaitPhase;
}
