#include "AnimationPlayer.h"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

AnimationPlayer::AnimationPlayer()
{
	mGaitCycle = new GaitCycle;
	mGaitPhase=0.0f;
	mStrideLength=1.0f;
	mSwingHeight=1.0f;
	m_easing = EASING_STATE::COSINE_INV_NORM;
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
		mGaitCycle->mGaitPeriod = 2.0f;
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

float AnimationPlayer::easeCosineInvNorm( float p_t )
{
	return (cos(p_t*2.0f*M_PI)-1.0f)*-0.5f;
}

float AnimationPlayer::easeHalfSine( float p_t )
{
	return sin(p_t*M_PI);
}

AnimationPlayer::EASING_STATE* AnimationPlayer::getEasingStateRef()
{
	return &m_easing;
}

void AnimationPlayer::setEasingState( EASING_STATE p_state )
{
	m_easing=p_state;
}

float AnimationPlayer::autoEase( float p_t )
{
	switch(m_easing)
	{
	case COSINE_INV_NORM:
		return easeCosineInvNorm(p_t);
	case HALF_SINE:
		return easeHalfSine(p_t);
	default:
		return easeCosineInvNorm(p_t);
	}
}

void AnimationPlayer::setStrideLength( float p_value )
{
	mStrideLength=p_value;
}

void AnimationPlayer::setSwingHeight( float p_value )
{
	mSwingHeight=p_value;
}

float AnimationPlayer::getStrideLength()
{
	return mStrideLength;
}

float AnimationPlayer::getSwingHeight()
{
	return mSwingHeight;
}

float* AnimationPlayer::getStrideLengthRef()
{
	return &mStrideLength;
}

float* AnimationPlayer::getSwingHeightRef()
{
	return &mSwingHeight;
}

