#include <unistd.h>
#include <math.h>

#include "MIDICLClient.h"
#include "MIDICLOutputPort.h"

void
ApplierFunction (const void *inKey, const void *inValue, void *inContext);

int
main (int argc, char *argv [])
{
	int	numDestinations = MIDIGetNumberOfDestinations ();

	printf ("MRPShell: found %d MIDI destinations \n", numDestinations);

	for (int i = 0; i < numDestinations; i++)
	{
		CFPropertyListRef	properties;
		MIDIDeviceRef	device;
		MIDIEntityRef	entity;

		MIDIEndpointRef	endpoint = MIDIGetDestination (i);

		OSStatus	status = MIDIObjectGetProperties (endpoint, &properties, false);

		if (status == 0)
		{
			CFIndex	index = CFDictionaryGetCount ((CFDictionaryRef) properties);

			printf ("GetCount returns %ld \n", index);

			CFDictionaryApplyFunction ((CFDictionaryRef) properties, ApplierFunction, NULL);
		}
		else
		{
			printf ("error %ld calling GetProperties \n", status);
		}
	}

	return 0;
}

void
ApplierFunction (const void *inKey, const void *inValue, void *inContext)
{
	CFStringRef	key = (CFStringRef) inKey;
	CFStringRef	value = (CFStringRef) inValue;

	const char	*cKey = CFStringGetCStringPtr (key, 0);
	const char	*cValue = CFStringGetCStringPtr (value, 0);

	printf ("%s=%s \n", cKey ? cKey : "none", cValue ? cValue : "none");
}



