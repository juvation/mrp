#ifndef MRPMIDIClock_h
#define MRPMIDIClock_h

// NAMESPACE

using namespace std;

// INCLUDES

#include "MRPSequencerClock.h"
#include "MIDICLInputPortListener.h"

// FORWARD DECLARATIONS

class MIDICLInputPort;
class MRPSequencer;

// MRPMIDIClock

class MRPMIDIClock
	:
	public MRPSequencerClock,
	public MIDICLInputPortListener
{
	// public constructor
	public:

		MRPMIDIClock (MRPSequencer *inSequencer, MIDICLInputPort *inInputPort);

		~MRPMIDIClock ();

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

	// MIDICLInputPortListener implementation
	public:

		void
		Hear (const MIDIPacketList *inPacketList);

	// static protected data
	protected:

		static const int
		kTicksPerQuarterNote = 24;

	// protected data
	protected:

		MIDICLInputPort *
		mInputPort;

		unsigned long
		mTimerPeriodUS;

		double
		mBPM;

};

#endif	// MRPMIDIClock_h


