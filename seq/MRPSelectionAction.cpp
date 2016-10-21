// MRPSelectionAction.cpp

#include "MRPSelectionAction.h"

#include "MRPSequencer.h"

// FORWARD DECLARATIONS

class MRPSequencer;

// CLASS DECLARATIONS

// MRPSELECTIONACTION

MRPSelectionAction::~MRPSelectionAction ()
{
}

// MRPSELECTIONCHANGECHANNEL

MRPSelectionChangeChannel::MRPSelectionChangeChannel
	(int inFromPort, int inFromChannel, int inToPort, int inToChannel)
	:
	mFromChannel (inFromChannel),
	mFromPort (inFromPort),
	mToChannel (inToChannel),
	mToPort (inToPort)
{
}

void
MRPSelectionChangeChannel::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	MRPEvent	&event (inSequencer->GetEvent (inTrack1, inStep1));

	if (mFromPort == -1 || (event.GetPort () == mFromPort))
	{
		if (event.GetChannel () == mFromChannel)
		{
			if (mToPort != -1)
			{
				inSequencer->SetPort (inTrack1, inStep1, mToPort);
			}

			inSequencer->SetChannel (inTrack1, inStep1, mToChannel);
		}
	}
}

// MRPSELECTIONCHANGENOTE

MRPSelectionChangeNote::MRPSelectionChangeNote
	(unsigned char inFromNote, unsigned char inToNote)
	:
	mFromNote (inFromNote),
	mToNote (inToNote)
{
}

void
MRPSelectionChangeNote::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	MRPEvent	&event (inSequencer->GetEvent (inTrack1, inStep1));

	if (event.GetData1 () == mFromNote)
	{
		inSequencer->SetNote (inTrack1, inStep1, mToNote);
	}
}

// MRPSELECTIONCLEAR

void
MRPSelectionClear::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->UnSelectEvent (inTrack1, inStep1);
}

// MRPSELECTIONHSOLO

void
MRPSelectionHSolo::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->HSolo (inTrack1, inStep1);
}

// MRPSELECTIONHUNSOLO

void
MRPSelectionHUnSolo::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->HUnSolo (inTrack1, inStep1);
}

// MRPSELECTIONINCREMENTCHANNEL

MRPSelectionIncrementChannel::MRPSelectionIncrementChannel (int inDelta)
	:
	mDelta (inDelta)
{
}
		
void
MRPSelectionIncrementChannel::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	channel (inSequencer->GetEvent (inTrack1, inStep1).GetChannel () + mDelta);

	if (channel >= 0 && channel <= 15)
	{
		inSequencer->SetChannel (inTrack1, inStep1, channel);
	}
}

// MRPSELECTIONINCREMENTGATETIME

MRPSelectionIncrementGateTime::MRPSelectionIncrementGateTime (int inDelta)
	:
	mDelta (inDelta)
{
}
		
void
MRPSelectionIncrementGateTime::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	gate (inSequencer->GetEvent (inTrack1, inStep1).GetGateTime () + mDelta);

	if (gate >= 1 && gate <= 90)
	{
		inSequencer->SetGateTime (inTrack1, inStep1, gate);
	}
}

// MRPSELECTIONINCREMENTNOTE

MRPSelectionIncrementNote::MRPSelectionIncrementNote (int inDelta)
	:
	mDelta (inDelta)
{
}
		
void
MRPSelectionIncrementNote::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	note (inSequencer->GetEvent (inTrack1, inStep1).GetData1 () + mDelta);

	if (note >= 0 && note < 128)
	{
		inSequencer->SetNote (inTrack1, inStep1, note);
	}
}

// MRPSELECTIONINCREMENTPORT

MRPSelectionIncrementPort::MRPSelectionIncrementPort (int inDelta)
	:
	mDelta (inDelta)
{
}
		
void
MRPSelectionIncrementPort::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	port (inSequencer->GetEvent (inTrack1, inStep1).GetPort () + mDelta);

	if (port >= 0 && port < inSequencer->GetOutputPortCount ())
	{
		inSequencer->SetPort (inTrack1, inStep1, port);
	}
}

// MRPSELECTIONINCREMENTVELOCITY

MRPSelectionIncrementVelocity::MRPSelectionIncrementVelocity (int inDelta)
	:
	mDelta (inDelta)
{
}
		
void
MRPSelectionIncrementVelocity::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	velocity (inSequencer->GetEvent (inTrack1, inStep1).GetData2 () + mDelta);

	if (velocity >= 0 && velocity < 128)
	{
		inSequencer->SetVelocity (inTrack1, inStep1, velocity);
	}
}

// MRPSELECTIONINVERTMUTE

void
MRPSelectionInvertMute::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->InvertMute (inTrack1, inStep1);
}

// MRPSELECTIONMUTE

void
MRPSelectionMute::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->Mute (inTrack1, inStep1);
}

// MRPSELECTIONRANDOMISECHANNEL

MRPSelectionRandomiseChannel::MRPSelectionRandomiseChannel (int inBase, int inRange)
	:
	mBase (inBase),
	mRange (inRange)
{
}
		
void
MRPSelectionRandomiseChannel::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	channel (rand () % (mRange + 1));

	channel += mBase;

	inSequencer->SetChannel (inTrack1, inStep1, channel);
}

// MRPSELECTIONRANDOMISEDATA1

MRPSelectionRandomiseData1::MRPSelectionRandomiseData1 (int inBase, int inRange)
	:
	mBase (inBase),
	mRange (inRange)
{
}
		
void
MRPSelectionRandomiseData1::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	data1 (rand () % (mRange + 1));

	data1 += mBase;

	inSequencer->SetData1 (inTrack1, inStep1, data1);
}

// MRPSELECTIONRANDOMISEDATA2

MRPSelectionRandomiseData2::MRPSelectionRandomiseData2 (int inBase, int inRange)
	:
	mBase (inBase),
	mRange (inRange)
{
}
		
void
MRPSelectionRandomiseData2::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	data2 (rand () % (mRange + 1));

	data2 += mBase;

	inSequencer->SetData2 (inTrack1, inStep1, data2);
}

// MRPSELECTIONRANDOMISEGATE

MRPSelectionRandomiseGate::MRPSelectionRandomiseGate (int inBase, int inRange)
	:
	mBase (inBase),
	mRange (inRange)
{
}
		
void
MRPSelectionRandomiseGate::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	gate (rand () % (mRange + 1));

	gate += mBase;

	inSequencer->SetGateTime (inTrack1, inStep1, gate);
}

// MRPSELECTIONRANDOMMUTE

void
MRPSelectionRandomMute::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	bool	mute ((rand () % 2) != 0);

	inSequencer->SetMute (inTrack1, inStep1, mute);
}

// MRPSELECTIONRANDOMISENOTE

MRPSelectionRandomiseNote::MRPSelectionRandomiseNote (int inBase, int inRange)
	:
	mBase (inBase),
	mRange (inRange)
{
}
		
void
MRPSelectionRandomiseNote::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	note (rand () % (mRange + 1));

	note += mBase;

	inSequencer->SetNote (inTrack1, inStep1, note);
}

// MRPSELECTIONRANDOMISEPORT

MRPSelectionRandomisePort::MRPSelectionRandomisePort (int inBase, int inRange)
	:
	mBase (inBase),
	mRange (inRange)
{
}
		
void
MRPSelectionRandomisePort::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	port (rand () % (mRange + 1));

	port += mBase;

	inSequencer->SetPort (inTrack1, inStep1, port);
}

// MRPSELECTIONRANDOMISEVELOCITY

MRPSelectionRandomiseVelocity::MRPSelectionRandomiseVelocity (int inBase, int inRange)
	:
	mBase (inBase),
	mRange (inRange)
{
}
		
void
MRPSelectionRandomiseVelocity::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	velocity (rand () % (mRange + 1));

	velocity += mBase;

	inSequencer->SetVelocity (inTrack1, inStep1, velocity);
}

// MRPSELECTIONSELECTBYCLOCKDIVISION

MRPSelectionSelectByClockDivision::MRPSelectionSelectByClockDivision
	(int inDivision)
	:
	mDivision (inDivision)
{
}
		
void
MRPSelectionSelectByClockDivision::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	int	stepDivision (inSequencer->GetStepCount () / inSequencer->GetBarCount ());
	
	stepDivision /= mDivision;

	if ((inStep1 % stepDivision) == 1)
	{
		inSequencer->SelectEvent (inTrack1, inStep1);
	}
	else
	{
		inSequencer->UnSelectEvent (inTrack1, inStep1);
	}
}

// MRPSELECTIONSETCHANNEL

MRPSelectionSetChannel::MRPSelectionSetChannel (unsigned char inChannel)
	:
	mChannel (inChannel)
{
}
		
void
MRPSelectionSetChannel::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->SetChannel (inTrack1, inStep1, mChannel);
}

// MRPSELECTIONSETDATA1

MRPSelectionSetData1::MRPSelectionSetData1 (unsigned char inData1)
	:
	mData1 (inData1)
{
}
		
void
MRPSelectionSetData1::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->SetData1 (inTrack1, inStep1, mData1);
}

// MRPSELECTIONSETDATA2

MRPSelectionSetData2::MRPSelectionSetData2 (unsigned char inData2)
	:
	mData2 (inData2)
{
}
		
void
MRPSelectionSetData2::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->SetData2 (inTrack1, inStep1, mData2);
}

// MRPSELECTIONSETGATETIME

MRPSelectionSetGateTime::MRPSelectionSetGateTime (unsigned char inGateTime)
	:
	mGateTime (inGateTime)
{
}
		
void
MRPSelectionSetGateTime::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->SetGateTime (inTrack1, inStep1, mGateTime);
}

// MRPSELECTIONSETNOTE

MRPSelectionSetNote::MRPSelectionSetNote (unsigned char inNote)
	:
	mNote (inNote)
{
}
		
void
MRPSelectionSetNote::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->SetNote (inTrack1, inStep1, mNote);
}

// MRPSELECTIONSETPORT

MRPSelectionSetPort::MRPSelectionSetPort (unsigned char inPort)
	:
	mPort (inPort)
{
}
		
void
MRPSelectionSetPort::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->SetPort (inTrack1, inStep1, mPort);
}

// MRPSELECTIONSETTYPE

MRPSelectionSetType::MRPSelectionSetType (unsigned char inCommandByte)
	:
	mCommandByte (inCommandByte)
{
}
		
void
MRPSelectionSetType::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->SetType (inTrack1, inStep1, mCommandByte);
}

// MRPSELECTIONSETVELOCITY

MRPSelectionSetVelocity::MRPSelectionSetVelocity (unsigned char inVelocity)
	:
	mVelocity (inVelocity)
{
}
		
void
MRPSelectionSetVelocity::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->SetVelocity (inTrack1, inStep1, mVelocity);
}

// MRPSELECTIONUNMUTE

void
MRPSelectionUnMute::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->UnMute (inTrack1, inStep1);
}

// MRPSELECTIONVSOLO

void
MRPSelectionVSolo::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->VSolo (inTrack1, inStep1);
}

// MRPSELECTIONVUNSOLO

void
MRPSelectionVUnSolo::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	inSequencer->VUnSolo (inTrack1, inStep1);
}

