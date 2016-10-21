// MIDICLMonitor.h

// GUARD

#ifndef MIDICLMonitor_h
#define MIDICLMonitor_h

// INCLUDES

#include "MIDICLInputPortListener.h"

// FORWARD DECLARATIONS

class MIDICLOutputPort;
struct MIDIPacketList;

// CLASS

class MIDICLMonitor
	:
	public MIDICLInputPortListener
{
	// MIDICLInputPortListener implementation
	public:

		void
		Hear (const MIDIPacketList *inList);
};

#endif	// MIDICLMonitor_h

