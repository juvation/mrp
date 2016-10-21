// MIDICLProcessingListener.h

// GUARD

#ifndef MIDICLProcessingListener_h
#define MIDICLProcessingListener_h

// INCLUDES

#include "MIDICLInputPortListener.h"

#include <CoreMIDI/MIDIServices.h>

// FORWARD DECLARATIONS

class MIDICLOutputPort;
struct MIDIPacketList;

// CLASS

class MIDICLProcessingListener
	:
	public MIDICLInputPortListener
{
	// public constructors/destructor
	public:

		MIDICLProcessingListener (MIDICLOutputPort *outPort);

	// public pure virtual methods
	public:

		virtual UInt16
		Process (const MIDIPacket *inInputPacket, Byte *outOutputPacket) = 0;

	// MIDICLInputPortListener implementation
	public:

		void
		Hear (const MIDIPacketList *inList);

	// private data
	private:

		MIDICLOutputPort *
		mOutputPort;
};

#endif	// MIDICLProcessingListener_h

