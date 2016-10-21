// MIDICLClient.h

// GUARD

#ifndef MIDICLClient_h
#define MIDICLClient_h

#include <CoreMIDI/CoreMIDI.h>

// INCLUDES

// FORWARD DECLARATIONS

class MIDICLDestination;
class MIDICLInputPort;
class MIDICLOutputPort;

// CLASS

class MIDICLClient
{
	// public constants
	public:

		static const int
		kMIDINoteOffMessage;

		static const int
		kMIDINoteOnMessage;

		static const int
		kMIDIPolyAftertouchMessage;

		static const int
		kMIDIControlChangeMessage;

		static const int
		kMIDIProgramChangeMessage;

		static const int
		kMIDIChannelAftertouchMessage;

		static const int
		kMIDIPitchBendMessage;

		static const int
		kMIDIStartSysExMessage;

		static const int
		kMIDIStopSysExMessage;

		static const int
		kMIDIClockMessage;

		static const int
		kMIDIStartMessage;

		static const int
		kMIDIContinueMessage;

		static const int
		kMIDIStopMessage;

		static const int
		kMIDIActiveSensingMessage;

		static const int
		kMIDIResetMessage;

	// public constructors/destructor
	public:

		MIDICLClient (CFStringRef inName);
		~MIDICLClient ();

	// public methods
	public:

		MIDICLDestination *
		MakeDestination (CFStringRef inName);

		MIDICLInputPort *
		MakeInputPort (CFStringRef inName);

		MIDICLOutputPort *
		MakeOutputPort (CFStringRef inName);

	// static private methods
	private:

		static void
		NotifyProc (const MIDINotification *inMessage, void *inRefCon);

	// private constructors
	private:

		MIDICLClient (const MIDICLClient &inCopy);

	// private operators overloaded
	private:

		MIDICLClient &
		operator = (const MIDICLClient &inCopy);
	
	// private data
	private:

		MIDIClientRef
		mClientRef;
};

#endif	// MIDICLClient_h


