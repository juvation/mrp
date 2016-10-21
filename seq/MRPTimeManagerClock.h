#ifndef MRPTimeManagerClock_h
#define MRPTimeManagerClock_h

// NAMESPACE

using namespace std;

// INCLUDES

#include "MRPSequencerClock.h"

#include <CoreServices/CoreServices.h>

// FORWARD DECLARATIONS

class MRPSequencer;
class MRPTimeManagerClock;

// TYPEDEFS

struct	MRPSequencerTimerTask
	:
	public TMTask
{
	public:

		MRPTimeManagerClock	*
		mClock;

};

// MRPTimeManagerClock

class MRPTimeManagerClock
	:
	public MRPSequencerClock
{
	// public constructor
	public:

		MRPTimeManagerClock (MRPSequencer *inSequencer);

		~MRPTimeManagerClock ();

	// MRPSequencerClock implementation
	public:

		double
		GetBPM () const;

		int
		SetBPM (double inBPM);

		int
		Start ();

		void
		Stop ();

	// static protected methods
	protected:

		static void
		TimeManagerTask (TMTaskPtr inTaskPtr);

	// static protected data
	protected:

		static const int
		kTicksPerQuarterNote = 24;

	// protected data
	protected:

		unsigned long
		mTimerPeriodUS;

		double
		mBPM;

		MRPSequencerTimerTask
		mTimeManagerTask;

		TimerUPP
		mTimeManagerUPP;
};

#endif	// MRPTimeManagerClock_h


