#include <unistd.h>
#include <math.h>

#include "MIDICLClient.h"
#include "MIDICLOutputPort.h"

int
main (int argc, char *argv [])
{
	MIDICLClient	midiClient (CFSTR ("MRPSequencer"));
	MIDICLOutputPort	*outputPort (NULL);

	int	numDestinations = MIDIGetNumberOfDestinations ();

	printf ("MRPShell: found %d MIDI destinations \n", numDestinations);

	if (numDestinations > 0)
	{
		puts ("MRPShell: opening output port");

		outputPort = midiClient.MakeOutputPort (CFSTR ("output"));
		outputPort->SetDestination (MIDIGetDestination (0));
	}

	for (UInt32 delay = 0; delay < 10; delay++)
	{
		UInt32	finalTicks (0);

		printf ("setting delay to %ld ticks \n", delay);

		// note-on
		outputPort->SendSmallPacket (0x90, 0x40, 0x90);

		for (int i = 0; i < 10; i++)
		{
			sleep (1);

			// note-off
			outputPort->SendSmallPacket (0x90, 0x40, 00);

			Delay (delay, &finalTicks);

			outputPort->SendSmallPacket (0x90, 0x40, 0x60);
		}

		Delay (delay, &finalTicks);

		// note-off
		outputPort->SendSmallPacket (0x90, 0x40, 00);
	}

	return 0;
}


