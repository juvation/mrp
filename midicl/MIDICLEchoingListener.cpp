// MIDICLEchoingListener.cpp

// INCLUDES

#include "MIDICLEchoingListener.h"

#include "MIDICLOutputPort.h"

#include <CoreMIDI/CoreMIDI.h>

// PUBLIC CONSTRUCTORS/DESTRUCTOR

MIDICLEchoingListener::MIDICLEchoingListener
	(MIDICLOutputPort *outPort)
	:
	mOutputPort (outPort)
{
}

// MIDICLINPUTPORTLISTENER IMPLEMENTATION

void
MIDICLEchoingListener::Hear (const MIDIPacketList *inList)
{
	mOutputPort->SendPacketList (inList);
}

