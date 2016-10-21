// MIDICLInputPort.h

// GUARD

#ifndef MIDICLInputPort_h
#define MIDICLInputPort_h

// INCLUDES

#include <CoreMIDI/CoreMIDI.h>

// FORWARD DECLARATIONS

class MIDICLInputPortListener;

// CLASS

class MIDICLInputPort
{
	// public constructors/destructor
	public:

		MIDICLInputPort (MIDIClientRef inClientRef, CFStringRef inName);
		~MIDICLInputPort ();

	// public methods
	public:

		void
		SetListener (MIDICLInputPortListener *inListener);

		void
		SetSource (MIDIEndpointRef inSource);

	// static private methods
	private:

		static void
		ReadProc (const MIDIPacketList *inList, void *inReadProcRefCon,
			void *inSrcConnectionRefCon);

	// private constructors
	private:

		MIDICLInputPort (const MIDICLInputPort &inCopy);

	// private operators overloaded
	private:

		MIDICLInputPort &
		operator = (const MIDICLInputPort &inCopy);

	// private data
	private:

		MIDICLInputPortListener *
		mListener;

		MIDIPortRef
		mPortRef;
};

#endif	// MIDICLInputPort_h


