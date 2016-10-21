// MRPSequencerMIDIClock.h

#ifndef MRPSequencerMIDIClock_h
#define MRPSequencerMIDIClock_h

// INCLUDES

#include "MRPSequencerListener.h"

// FORWARD

class MRPSequencer;

class MIDICLOutputPort;

// MRPSequencerMIDIClock

class MRPSequencerMIDIClock
	:
	public MRPSequencerListener
{
	// public constructor
	public:

		MRPSequencerMIDIClock (MIDICLOutputPort *outPort);

	// public virtual destructor
	public:

		virtual ~
		MRPSequencerMIDIClock ();

	// public pure virtual methods
	public:

		virtual void
		BPMChanged (MRPSequencer *inSequencer);

		virtual void
		ClockReset (MRPSequencer *inSequencer);

		virtual void
		ConfigChanged (MRPSequencer *inSequencer);

		virtual void
		EventChanged (MRPSequencer *inSequencer, int inTrack, int inStep);

		virtual void
		LoopChanged (MRPSequencer *inSequencer);

		virtual void
		Pause (MRPSequencer *inSequencer);

		virtual void
		PortListChanged (MRPSequencer *inSequencer);

		virtual void
		ScaleChanged (MRPSequencer *inSequencer);

		virtual void
		Start (MRPSequencer *inSequencer);

		virtual void
		StepTick (MRPSequencer *inSequencer);

		virtual void
		Stop (MRPSequencer *inSequencer);

		virtual void
		TimerTick (MRPSequencer *inSequencer);

		virtual void
		TrackChanged (MRPSequencer *inSequencer, int inTrack);

		virtual void
		TrackAdded (MRPSequencer *inSequencer, int inTrack);

		virtual void
		TrackDeleted (MRPSequencer *inSequencer, int inTrack);

	// private data
	private:

		MIDICLOutputPort *
		mOutputPort;

		bool
		mClockIsReset;
};

#endif	// MRPSequencerMIDIClock_h

