// MIDICLEndpoint.cpp

// INCLUDES

#include "MIDICLEndpoint.h"

// PUBLIC STATIC METHODS

void
MIDICLEndpoint::GetDestination
	(ItemCount inIndex0, MIDIEndpointRef *outEndpoint,
		char *outNameCString, CFIndex inBufferSize)
{
	if (inIndex0 < MIDIGetNumberOfDestinations ())
	{
		*outEndpoint = MIDIGetDestination (inIndex0);

		CFStringRef	pname;

		MIDIObjectGetStringProperty
			(*outEndpoint, kMIDIPropertyName, &pname);

		CFStringGetCString (pname, outNameCString, inBufferSize, 0);
		CFRelease (pname);
	}
	else
	{
		*outEndpoint = NULL;
	}
}

ItemCount
MIDICLEndpoint::GetNumberOfDestinations ()
{
	return MIDIGetNumberOfDestinations ();
}

ItemCount
MIDICLEndpoint::GetNumberOfSources ()
{
	return MIDIGetNumberOfSources ();
}

void
MIDICLEndpoint::GetSource
	(ItemCount inIndex0, MIDIEndpointRef *outEndpoint,
		char *outNameCString, CFIndex inBufferSize)
{
	if (inIndex0 < MIDIGetNumberOfSources ())
	{
		*outEndpoint = MIDIGetSource (inIndex0);

		CFStringRef	pname;

		MIDIObjectGetStringProperty
			(*outEndpoint, kMIDIPropertyName, &pname);

		CFStringGetCString (pname, outNameCString, inBufferSize, 0);
		CFRelease (pname);
	}
	else
	{
		*outEndpoint = NULL;
	}
}

