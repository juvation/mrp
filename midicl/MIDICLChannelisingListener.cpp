// MIDICLChannelisingListener.cpp

// INCLUDES

#include "MIDICLChannelisingListener.h"

#include "MIDICLOutputPort.h"

// PUBLIC CONSTRUCTORS/DESTRUCTOR

MIDICLChannelisingListener::MIDICLChannelisingListener
	(MIDICLOutputPort *outPort)
	:
	mOutputPort (outPort)
{
}

// MIDICLINPUTPORTLISTENER IMPLEMENTATION

void
MIDICLChannelisingListener::Hear (const MIDIPacketList *inPacketList)
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
		// copy & maybe channelise
		for (UInt16 i = 0; i < inputPacket->length; i++)
		{
			if (inputPacket->data [i] >= 0x80
				&& inputPacket->data [i] < 0xf0)
			{
				printf ("got status byte %02x \n", 
					inputPacket->data [i] & 0xff);

				// status byte
				// does the channel match?
				if (mFromChannel == 0
					|| (inputPacket->data [i] & 0xf) == mFromChannel)
				{
					outputPacketBuffer [i]
						= (inputPacket->data [i] & 0xf0) | mToChannel;
				}
				else
				{
					outputPacketBuffer [i] = inputPacket->data [i];
				}
			}
			else
			{
				outputPacketBuffer [i] = inputPacket->data [i];
			}
		}

		do
		{
			outputPacket = MIDIPacketListAdd
				(outputPacketList, sizeof (outputPacketListBuffer),
					outputPacket, 0, inputPacket->length, outputPacketBuffer);

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

// PUBLIC METHODS

void
MIDICLChannelisingListener::SetFromChannel (Byte inChannel)
{
	if (inChannel >= 1 && inChannel <= 16)
	{
		mFromChannel = inChannel;
	}
	else
	{
		mFromChannel = 0;
	}
}

void
MIDICLChannelisingListener::SetToChannel (Byte inChannel)
{
	if (inChannel >= 1 && inChannel <= 16)
	{
		mToChannel = inChannel;
	}
}


