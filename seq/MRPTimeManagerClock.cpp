#include "MRPTimeManagerClock.h"

using namespace std;

// INCLUDES

#include "MRPTimeManagerClock.h"

#include "MRPSequencer.h"

// PUBLIC CONSTRUCTOR/DESTRUCTOR

MRPTimeManagerClock::MRPTimeManagerClock (MRPSequencer *inSequencer)
	:
	MRPSequencerClock (inSequencer)
{
}

MRPTimeManagerClock::~MRPTimeManagerClock ()
{
	Stop ();
}

// MRPSEQUENCERCLOCK IMPLEMENTATION

double
MRPTimeManagerClock::GetBPM () const
{
	return mBPM;
}

int
MRPTimeManagerClock::SetBPM (double inBPM)
{
	mBPM = inBPM;

	double	bps = mBPM / 60;
	double	pps = bps * kTicksPerQuarterNote;

	mTimerPeriodUS = lround (1000000.0 / pps);

	return 0;
}

int
MRPTimeManagerClock::Start ()
{
	if (mStarted)
	{
		// necessary?
		Stop ();
	}

	mTimeManagerTask.tmAddr = NewTimerUPP
		(MRPTimeManagerClock::TimeManagerTask);
	mTimeManagerTask.tmCount = 0;
	mTimeManagerTask.tmWakeUp = 0;
	mTimeManagerTask.tmReserved = 0;
	mTimeManagerTask.mClock = this;

	OSErr errCode = InstallXTimeTask ((QElemPtr) &mTimeManagerTask);

	if (errCode == 0)
	{
		errCode = PrimeTimeTask
			((QElemPtr) &mTimeManagerTask, - (mTimerPeriodUS));
	}

	if (errCode == 0)
	{
		mStarted = true;
	}

	return errCode;
}

void
MRPTimeManagerClock::Stop ()
{
	if (mStarted)
	{
		mSequencer->SendAllNotesOff ();

		RemoveTimeTask ((QElemPtr) &mTimeManagerTask);
		DisposeTimerUPP (mTimeManagerTask.tmAddr);

		mStarted = false;
	}
}

// PROTECTED STATIC METHODS

void
MRPTimeManagerClock::TimeManagerTask (TMTaskPtr inTaskPtr)
{
	MRPSequencerTimerTask	*sequencerTask
		= (MRPSequencerTimerTask *) inTaskPtr;

	MRPTimeManagerClock	*self (sequencerTask->mClock);
	MRPSequencer	*sequencer (self->mSequencer);

	sequencer->TimerTick ();

	PrimeTimeTask
		((QElemPtr) inTaskPtr, - (self->mTimerPeriodUS));
}

