#include "MRPSequencerClock.h"

using namespace std;

// PUBLIC CONSTRUCTOR/DESTRUCTOR

MRPSequencerClock::MRPSequencerClock (MRPSequencer *inSequencer)
	:
	mSequencer (inSequencer),
	mStarted (false)
{
}

MRPSequencerClock::~MRPSequencerClock ()
{
}

