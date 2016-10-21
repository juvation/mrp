
#include <CoreMIDI/CoreMIDI.h>
#include <stdio.h>

int
main ()
{
	ItemCount	nv = MIDIGetNumberOfDevices ();

	printf ("%lu devices in system \n", nv);

	for (ItemCount v = 0; v < nv; v++)
	{
		CFStringRef	pname;
		char	name [64];

		MIDIDeviceRef	dr = MIDIGetDevice (v);

		if (1)
		{
			MIDIObjectGetStringProperty (dr, kMIDIPropertyName, &pname);
			CFStringGetCString (pname, name, sizeof (name), 0);
			printf ("device %lu name = '%s' \n", v, name);
			CFRelease (pname);
		}

		ItemCount	ne = MIDIDeviceGetNumberOfEntities (dr);

		printf ("\t%lu entities for this device \n", ne);

		for (ItemCount e = 0; e < ne; e++)
		{
			MIDIEntityRef	er = MIDIDeviceGetEntity (dr, e);

			if (1)
			{
				name [0] = 0;
				MIDIObjectGetStringProperty (er, kMIDIPropertyName, &pname);
				CFStringGetCString (pname, name, sizeof (name), 0);
				printf ("\tentity %lu name = '%s' \n", v, name);
				CFRelease (pname);
			}

			ItemCount	ns = MIDIEntityGetNumberOfSources (er);

			printf ("\t\t%lu sources for this entity \n", ns);

			for (ItemCount s = 0; s < ns; s++)
			{
				MIDIEndpointRef	pf = MIDIEntityGetSource (er, s);

				if (1)
				{
					name [0] = 0;
					MIDIObjectGetStringProperty (pf, kMIDIPropertyName, &pname);
					CFStringGetCString (pname, name, sizeof (name), 0);
					printf ("\t\tsource %lu name = '%s' \n", v, name);
					CFRelease (pname);
				}
			}

			ItemCount	nd = MIDIEntityGetNumberOfDestinations (er);

			printf ("\t\t%lu destinations for this entity \n", nd);

			for (ItemCount d = 0; d < nd; d++)
			{
				MIDIEndpointRef	pf = MIDIEntityGetDestination (er, d);

				if (1)
				{
					name [0] = 0;
					MIDIObjectGetStringProperty (pf, kMIDIPropertyName, &pname);
					CFStringGetCString (pname, name, sizeof (name), 0);
					printf ("\t\tdestination %lu name = '%s' \n", v, name);
					CFRelease (pname);
				}
			}
		}
	}

	return 0;
}

