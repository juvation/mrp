// MIDICLChannelisingListener.h

// GUARD

#ifndef MIDICLChannelisingListener_h
#define MIDICLChannelisingListener_h

// INCLUDES

#include "MIDICLInputPortListener.h"

#include <CoreMIDI/MIDIServices.h>

// FORWARD DECLARATIONS

class MIDICLOutputPort;
struct MIDIPacketList;

// CLASS

class MIDICLChannelisingListener
	:
	public MIDICLInputPortListener
{
	// public constructors/destructor
	public:

		MIDICLChannelisingListener (MIDICLOutputPort *outPort);

	// MIDICLInputPortListener implementation
	public:

		void
		Hear (const MIDIPacketList *inList);

	// public methods
	public:

		// anything out of the range 1-16
		// is taken to mean "any channel"
		void
		SetFromChannel (Byte inChannel);

		// anything out of the range 1-16
		// is ignored (no config change)
		void
		SetToChannel (Byte inChannel);

	// private data
	private:

		MIDICLOutputPort *
		mOutputPort;

		Byte
		mFromChannel;

		Byte
		mToChannel;
};

#endif	// MIDICLChannelisingListener_h

