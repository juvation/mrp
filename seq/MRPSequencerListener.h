// MRPSequencerListener.h

#ifndef MRPSequencerListener_h
#define MRPSequencerListener_h

// INCLUDES

// FORWARD

class MRPSequencer;

// MRPSEQUENCERLISTENER

class MRPSequencerListener
{
	// public constructor / destructor
	public:

		MRPSequencerListener ();

		virtual ~
		MRPSequencerListener ();

	// public virtual methods
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
		StepTickPost (MRPSequencer *inSequencer);

		virtual void
		Stop (MRPSequencer *inSequencer);

		virtual void
		TimerTick (MRPSequencer *inSequencer);

		virtual void
		TimerTickPost (MRPSequencer *inSequencer);

		virtual void
		TrackChanged (MRPSequencer *inSequencer, int inTrack);

		virtual void
		TrackAdded (MRPSequencer *inSequencer, int inTrack);

		virtual void
		TrackDeleted (MRPSequencer *inSequencer, int inTrack);

};

#endif	// MRPSequencerListener_h

