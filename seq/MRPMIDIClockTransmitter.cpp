// MRPSequencerMIDIClock.cpp

// INCLUDES

#include "MRPSequencerMIDIClock.h"

#include "MRPSequencer.h"

#include "MIDICLClient.h"
#include "MIDICLOutputPort.h"

// MRPSEQUENCERMIDICLOCK

// PUBLIC CONSTRUCTOR

MRPSequencerMIDIClock::MRPSequencerMIDIClock (MIDICLOutputPort *outPort)
	:
	mOutputPort (outPort),
	mClockIsReset (true)
{
}

// PUBLIC DESTRUCTOR

MRPSequencerMIDIClock::~MRPSequencerMIDIClock ()
{
}

// MRPSEQUENCERLISTENER INTERFACE

void
MRPSequencerMIDIClock::BPMChanged (MRPSequencer *inSequencer)
{
}

void
MRPSequencerMIDIClock::ClockReset (MRPSequencer *inSequencer)
{
	mClockIsReset = true;
}

void
MRPSequencerMIDIClock::ConfigChanged (MRPSequencer *inSequencer)
{
}

void
MRPSequencerMIDIClock::LoopChanged (MRPSequencer *inSequencer)
{
}

void
MRPSequencerMIDIClock::EventChanged (MRPSequencer *inSequencer, int inTrack, int inStep)
{
}

void
MRPSequencerMIDIClock::Pause (MRPSequencer *inSequencer)
{
	mOutputPort->SendStop ();
}

void
MRPSequencerMIDIClock::PortListChanged (MRPSequencer *inSequencer)
{
}

void
MRPSequencerMIDIClock::ScaleChanged (MRPSequencer *inSequencer)
{
}

void
MRPSequencerMIDIClock::Start (MRPSequencer *inSequencer)
{
	if (mClockIsReset)
	{
		mOutputPort->SendStart ();
		mClockIsReset = false;
	}
	else
	{
		mOutputPort->SendContinue ();
	}
}

void
MRPSequencerMIDIClock::StepTick (MRPSequencer *inSequencer)
{
	mOutputPort->SendClock ();
}

void
MRPSequencerMIDIClock::Stop (MRPSequencer *)
{
	mClockIsReset = true;

	mOutputPort->SendStop ();
}

void
MRPSequencerMIDIClock::TimerTick (MRPSequencer *)
{
	mOutputPort->SendClock ();
}

void
MRPSequencerMIDIClock::TrackChanged (MRPSequencer *, int inTrack)
{
}

void
MRPSequencerMIDIClock::TrackAdded (MRPSequencer *, int inTrack)
{
}

void
MRPSequencerMIDIClock::TrackDeleted (MRPSequencer *, int inTrack)
{
}


