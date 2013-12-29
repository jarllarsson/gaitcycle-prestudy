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
		mGaitCycle->mGaitPeriod = 1.0f;
		mGaitCycle->mStepCycles = new StepCycle[n];
		// Fill with appropriate data
		for (int i=0;i<n;i++)
		{
			StepCycle* stepCycle = &(mGaitCycle->mStepCycles[i]);
			stepCycle->mNormDutyFactor = 0.3f;
			stepCycle->mNormStepTrigger = ((float)i/(float)n);
			// safety clamp
			while (stepCycle->mNormStepTrigger+stepCycle->mNormDutyFactor > 1.0f)
				stepCycle->mNormStepTrigger -= stepCycle->mNormDutyFactor;
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
