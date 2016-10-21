// MIDICLEndpoint.h

// GUARD

#ifndef MIDICLEndpoint_h
#define MIDICLEndpoint_h

// INCLUDES

#include <CoreMIDI/CoreMIDI.h>

// CLASS

class MIDICLEndpoint
{
	// public static methods
	public:

		static void
		GetDestination (ItemCount inIndex0, MIDIEndpointRef *outEndpoint,
			char *outNameCString, CFIndex inBufferSize);

		static ItemCount
		GetNumberOfDestinations ();

		static ItemCount
		GetNumberOfSources ();

		static void
		GetSource (ItemCount inIndex0, MIDIEndpointRef *outEndpoint,
			char *outNameCString, CFIndex inBufferSize);

};

#endif // MIDICLEndpoint_h

