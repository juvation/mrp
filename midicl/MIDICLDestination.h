// MIDICLDestination.h

// GUARD

#ifndef MIDICLDestination_h
#define MIDICLDestination_h

// INCLUDES

#include <CoreMIDI/CoreMIDI.h>

// FORWARD DECLARATIONS

class MIDICLInputPortListener;

// CLASS

class MIDICLDestination
{
	// public constructors/destructor
	public:

		MIDICLDestination (MIDIClientRef inClientRef, CFStringRef inName);
		~MIDICLDestination ();

	// public methods
	public:

		void
		SetListener (MIDICLInputPortListener *inListener);

	// static private methods
	private:

		static void
		ReadProc (const MIDIPacketList *inList, void *inReadProcRefCon,
			void *inSrcConnectionRefCon);

	// private constructors
	private:

		MIDICLDestination (const MIDICLDestination &inCopy);

	// private operators overloaded
	private:

		MIDICLDestination &
		operator = (const MIDICLDestination &inCopy);

	// private data
	private:

		MIDICLInputPortListener *
		mListener;

		MIDIEndpointRef
		mDestinationRef;
};

#endif	// MIDICLDestination_h


