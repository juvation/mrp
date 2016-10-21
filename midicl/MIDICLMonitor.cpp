// MIDICLMonitor.cpp

// INCLUDES

#include "MIDICLMonitor.h"

#include <CoreMIDI/CoreMIDI.h>

// MIDICLINPUTPORTLISTENER IMPLEMENTATION

void
MIDICLMonitor::Hear (const MIDIPacketList *inPacketList)
{
	const MIDIPacket	*inputPacket (inPacketList->packet);

	for (UInt32	p = 0; p < inPacketList->numPackets;
		p++, inputPacket = MIDIPacketNext (inputPacket))
	{
		// copy & maybe channelise
		for (UInt16 i = 0; i < inputPacket->length; i++)
		{
			printf ("%02x ", inputPacket->data [i]);
		}

		printf ("\n");
	}
}

