
#include "MIDICLClient.h"
#include "MIDICLDestination.h"
#include "MIDICLException.h"
#include "MIDICLInputPort.h"
#include "MIDICLMonitor.h"
#include "MIDICLOutputPort.h"

#include <CoreFoundation/CoreFoundation.h>

// switch for listening on a destination instead of listening to an input port
#undef USE_DESTINATIONS

#ifdef USE_DESTINATIONS

int
main ()
{
	MIDICLDestination	*destination (NULL);
	MIDICLMonitor	*listener (NULL);

	try
	{
		MIDICLClient	client (CFSTR ("Monitor"));

		destination = client.MakeDestination (CFSTR ("whoopeeee"));

		listener = new MIDICLMonitor ();

		destination->SetListener (listener);

		CFRunLoopRun ();

	}
	catch (MIDICLException &inException)
	{
		printf ("error %ld \n", inException.GetErrCode ());
	}

	if (listener != NULL)
	{
		delete listener;
	}

	if (destination != NULL)
	{
		delete destination;
	}

	return 0;
}

#else

int
main ()
{
	MIDICLInputPort	*port (NULL);
	MIDICLMonitor	*listener (NULL);

	try
	{
		MIDICLClient	client (CFSTR ("Monitor"));

		port = client.MakeInputPort (CFSTR ("whoopeeee"));

		port->SetSource (MIDIGetSource (0));

		listener = new MIDICLMonitor ();

		port->SetListener (listener);

		CFRunLoopRun ();

	}
	catch (MIDICLException &inException)
	{
		printf ("error %ld \n", inException.GetErrCode ());
	}

	if (listener != NULL)
	{
		delete listener;
	}

	if (port != NULL)
	{
		delete port;
	}

	return 0;
}

#endif
