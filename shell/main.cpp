#include "MRPShell.h"
#include "MRPSequencer.h"
#include "MRPStdioInterface.h"

#include "MIDICLClient.h"
#include "MIDICLClient.h"
#include "MIDICLDestination.h"
#include "MIDICLInputPort.h"
#include "MIDICLOutputPort.h"

// MAINLINE

int
main (int inArgc, char *inArgv [])
{
	MIDICLDestination	*dummyDestination = NULL;

	double	bpm = 120;

	if (inArgc > 1)
	{
		bpm = atof (inArgv [1]);

		if (bpm == 0)
		{
			bpm = 120;
		}
	}

	MRPSequencer	sequencer;

	sequencer.SetScale (1, 16);
	sequencer.SetBPM (bpm);

	printf ("MRPShell: setting up MIDI config\n");

	{
		MIDICLClient	midiClient (CFSTR ("MRPSequencer"));

		int	numSources = MIDIGetNumberOfSources ();
		int	numDestinations = MIDIGetNumberOfDestinations ();

		printf ("MRPShell: found %d MIDI sources and %d destinations\n",
			numSources, numDestinations);

		if (numDestinations == 0)
		{
			printf ("MRPShell: no MIDI destinations found, making a dummy one\n");

			dummyDestination = midiClient.MakeDestination
				(CFSTR ("MRPShell dummy destination"));
		}

		for (int i = 0; i < numSources; i++)
		{
			MIDICLInputPort	*inputPort (midiClient.MakeInputPort (CFSTR ("input")));
			inputPort->SetSource (MIDIGetSource (i));

			sequencer.AddInputPort (inputPort);
		}

		for (int i = 0; i < numDestinations; i++)
		{
			MIDICLOutputPort	*outputPort (midiClient.MakeOutputPort (CFSTR ("output")));
			outputPort->SetDestination (MIDIGetDestination (i));

			sequencer.AddOutputPort (outputPort);
		}

		MRPStdioInterface	interface (stdin, stdout);
		MRPShell	shell (&sequencer, &interface);

		shell.Shell (true);

		if (sequencer.IsPlaying ())
		{
			sequencer.Stop ();
		}
	}

	// ensure the client goes away before we do - sigh
	if (dummyDestination != NULL)
	{
		delete dummyDestination;
	}

	return 0;
}

