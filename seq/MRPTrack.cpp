// MRPTrack.cpp

#include "MRPTrack.h"

// PUBLIC CONSTRUCTORS

MRPTrack::MRPTrack ()
	:
	mMuted (false),
	mHSoloCount (0),
	mEventCount (0)
{
}

MRPEvent &
MRPTrack::GetEvent (int inStep1)
{
	int	step0 (inStep1 - 1);

	return mEvents [step0];
}

const MRPEvent &
MRPTrack::GetEvent (int inStep1) const
{
	int	step0 (inStep1 - 1);

	return mEvents [step0];
}

MRPEventStatus &
MRPTrack::GetEventStatus (int inStep1)
{
	int	step0 (inStep1 - 1);

	return mEventStatus [step0];
}

const MRPEventStatus &
MRPTrack::GetEventStatus (int inStep1) const
{
	int	step0 (inStep1 - 1);

	return mEventStatus [step0];
}

int
MRPTrack::GetHSoloCount () const
{
	return mHSoloCount;
}

void
MRPTrack::Mute ()
{
	mMuted = true;
}

void
MRPTrack::UnMute ()
{
	mMuted = false;
}

void
MRPTrack::Mute (int inStep1)
{
	int	step0 (inStep1 - 1);

	mEventStatus [step0].mMuted = true;
}

void
MRPTrack::HSolo (int inStep1)
{
	int	step0 (inStep1 - 1);

	if (! mEventStatus [step0].mHSoloed)
	{
		mEventStatus [step0].mHSoloed = true;

		mHSoloCount++;
	}
}

void
MRPTrack::HUnSolo (int inStep1)
{
	int	step0 (inStep1 - 1);

	if (mEventStatus [step0].mHSoloed)
	{
		mEventStatus [step0].mHSoloed = false;

		mHSoloCount--;
	}
}

void
MRPTrack::UnMute (int inStep1)
{
	int	step0 (inStep1 - 1);

	mEventStatus [step0].mMuted = false;
}

