// MIDICLProcessingListener.cpp

// INCLUDES

#include "MIDICLProcessingListener.h"

#include "MIDICLOutputPort.h"

// PUBLIC CONSTRUCTORS/DESTRUCTOR

MIDICLProcessingListener::MIDICLProcessingListener
	(MIDICLOutputPort *outPort)
	:
	mOutputPort (outPort)
{
}

// MIDICLINPUTPORTLISTENER IMPLEMENTATION

void
MIDICLProcessingListener::Hear (const MIDIPacketList *inPacketList)
{
	Byte	outputPacketListBuffer [300];
	Byte	outputPacketBuffer [300];

	MIDIPacketList	*outputPacketList
		((MIDIPacketList *) outputPacketListBuffer);

	MIDIPacket	*outputPacket
		(MIDIPacketListInit (outputPacketList));

	const MIDIPacket	*inputPacket (inPacketList->packet);

	for (UInt32	p = 0; p < inPacketList->numPackets;
		p++, inputPacket = MIDIPacketNext (inputPacket))
	{
		// call the subclass to reprocess
		UInt16	length = Process (inputPacket, outputPacketBuffer);

		do
		{
			outputPacket = MIDIPacketListAdd
				(outputPacketList, sizeof (outputPacketListBuffer),
					outputPacket, 0, length, outputPacketBuffer);

			if (outputPacket == NULL)
			{
				// send what we have
				mOutputPort->SendPacketList (outputPacketList);

				// reinitialise the packet list
				outputPacket = MIDIPacketListInit (outputPacketList);
			}
		}
		while (outputPacket == NULL);
	}

	// send whatever remains
	mOutputPort->SendPacketList (outputPacketList);
}

