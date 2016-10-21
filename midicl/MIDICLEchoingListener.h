// MIDICLEchoingListener.h

// GUARD

#ifndef MIDICLEchoingListener_h
#define MIDICLEchoingListener_h

// INCLUDES

#include "MIDICLInputPortListener.h"

// FORWARD DECLARATIONS

class MIDICLOutputPort;
struct MIDIPacketList;

// CLASS

class MIDICLEchoingListener
	:
	public MIDICLInputPortListener
{
	// public constructors/destructor
	public:

		MIDICLEchoingListener (MIDICLOutputPort *outPort);

	// MIDICLInputPortListener implementation
	public:

		void
		Hear (const MIDIPacketList *inList);

	// private data
	private:

		MIDICLOutputPort *
		mOutputPort;
};

#endif	// MIDICLEchoingListener_h

