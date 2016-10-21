#include "MRPEventStatus.h"

using namespace std;

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <sstream>

// PUBLIC CONSTRUCTORS

MRPEventStatus::MRPEventStatus ()
{
	mHSoloed = false;
	mIsNoteOn = false;
	mMuted = false;
	mSelected = false;
	mVSoloed = false;

	mChannelPlaying = 0;
	mNotePlaying = 0;
	mPortPlaying = 0;
}

