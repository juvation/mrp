// MRPSequencerSMF.h

#ifndef MRPSequencerSMF_h
#define MRPSequencerSMF_h

// INCLUDES

#include <AudioToolbox/MusicPlayer.h>
#include <AudioToolbox/AudioToolbox.h>

// FORWARD

class MRPSequencer;
class MRPShellInterface;

// MRPSEQUENCERSMF

class MRPSequencerSMF
{
	// public constructor
	public:

		MRPSequencerSMF (MRPSequencer *inSequencer, MRPShellInterface *mInterface);

	// public methods
	public:

		int
		Export (const char *inFileName);

		int
		Import (const char *inFileName);

	// private methods
	private:

		void
		ClearTrack (MusicTrack inTrack);

		double
		GetTempo (MusicSequence inSequence);

		int
		LoadTrack (MusicTrack inTrack);

	// private data
	private:

		MRPSequencer *
		mSequencer;

		MRPShellInterface *
		mInterface;
};

#endif	// MRPSequencerSMF_h


