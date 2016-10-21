// MIDICLInputPortListener.h

// GUARD

#ifndef MIDICLInputPortListener_h
#define MIDICLInputPortListener_h

// FORWARD DECLARATIONS

struct MIDIPacketList;

// CLASS

class MIDICLInputPortListener
{
	// public virtual destructor
	public:

		virtual ~
		MIDICLInputPortListener ();

	// public pure virtual methods
	public:

		virtual void
		Hear (const MIDIPacketList *inList) = 0;
};

#endif	// MIDICLInputPortListener_h

