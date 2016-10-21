#include "MRPMIDIClock.h"

using namespace std;

// INCLUDES

#include "MRPMIDIClock.h"

#include "MIDICLClient.h"
#include "MIDICLInputPort.h"
#include "MRPSequencer.h"

// PUBLIC CONSTRUCTOR/DESTRUCTOR

MRPMIDIClock::MRPMIDIClock (MRPSequencer *inSequencer, MIDICLInputPort *inInputPort)
	:
	MRPSequencerClock (inSequencer),
	mInputPort (inInputPort)
{
	mInputPort->SetListener (this);
}

MRPMIDIClock::~MRPMIDIClock ()
{
	Stop ();

	mInputPort->SetListener (NULL);
}

// MRPSEQUENCERCLOCK IMPLEMENTATION

double
MRPMIDIClock::GetBPM () const
{
	// TODO - figure out BPM from analysing clock period
	return -1;
}

int
MRPMIDIClock::SetBPM (double inBPM)
{
	// can't set bpm under external clock control
	return -1;
}

int
MRPMIDIClock::Start ()
{
	return 0;
}

void
MRPMIDIClock::Stop ()
{
}

// MIDICLINPUTPORTLISTENER IMPLEMENTATION

void
MRPMIDIClock::Hear (const MIDIPacketList *inPacketList)
{
	const MIDIPacket  *inputPacket (inPacketList->packet);

	for (UInt32	p = 0; p < inPacketList->numPackets;
		p++, inputPacket = MIDIPacketNext (inputPacket))
	{
		for (UInt16 i = 0; i < inputPacket->length; i++)
		{
			Byte	command = inputPacket->data [i];

			if (command == MIDICLClient::kMIDIStartMessage)
			{
				mSequencer->Reset ();

				mStarted = true;
			}
			else if (command == MIDICLClient::kMIDIContinueMessage)
			{
				mStarted = true;
			}
			else if (command == MIDICLClient::kMIDIStopMessage)
			{
				mSequencer->SendAllNotesOff ();

				mStarted = false;
			}
			else if (command == MIDICLClient::kMIDIClockMessage)
			{
				if (mStarted)
				{
					mSequencer->TimerTick ();
				}
			}
		}
	}
}

