// MIDICLClient.cpp

// INCLUDES

#include "MIDICLClient.h"

#include "MIDICLDestination.h"
#include "MIDICLException.h"
#include "MIDICLInputPort.h"
#include "MIDICLOutputPort.h"

#include <CoreFoundation/CFRunLoop.h>

// STATIC INITIALISATION

const int
MIDICLClient::kMIDINoteOffMessage = 0x80;

const int
MIDICLClient::kMIDINoteOnMessage = 0x90;

const int
MIDICLClient::kMIDIPolyAftertouchMessage = 0xa0;

const int
MIDICLClient::kMIDIControlChangeMessage = 0xb0;

const int
MIDICLClient::kMIDIProgramChangeMessage = 0xc0;

const int
MIDICLClient::kMIDIChannelAftertouchMessage = 0xd0;

const int
MIDICLClient::kMIDIPitchBendMessage = 0xe0;

const int
MIDICLClient::kMIDIStartSysExMessage = 0xf0;

const int
MIDICLClient::kMIDIStopSysExMessage = 0xf7;

const int
MIDICLClient::kMIDIClockMessage = 0xf8;

const int
MIDICLClient::kMIDIStartMessage = 0xfa;

const int
MIDICLClient::kMIDIContinueMessage = 0xfb;

const int
MIDICLClient::kMIDIStopMessage = 0xfc;

const int
MIDICLClient::kMIDIActiveSensingMessage = 0xfe;

const int
MIDICLClient::kMIDIResetMessage = 0xff;

// PUBLIC CONSTRUCTORS/DESTRUCTOR

MIDICLClient::MIDICLClient (CFStringRef inName)
	// throws MIDICLException
{
	OSStatus	errCode = MIDIClientCreate
		(inName, NotifyProc, this, &mClientRef);

	if (errCode != 0)
	{
		throw MIDICLException
			(MIDICLException::kMIDIClientCreate, errCode);
	}
}

MIDICLClient::~MIDICLClient ()
{
	// this bins all the ports too
	MIDIClientDispose (mClientRef);
}

// PUBLIC METHODS

MIDICLDestination *
MIDICLClient::MakeDestination (CFStringRef inPortName)
	// throws MIDICLException
{
	return new MIDICLDestination (mClientRef, inPortName);
}

MIDICLInputPort *
MIDICLClient::MakeInputPort (CFStringRef inPortName)
	// throws MIDICLException
{
	return new MIDICLInputPort (mClientRef, inPortName);
}

MIDICLOutputPort *
MIDICLClient::MakeOutputPort (CFStringRef inPortName)
	// throws MIDICLException
{
	return new MIDICLOutputPort (mClientRef, inPortName);
}

// STATIC PRIVATE METHODS

void
MIDICLClient::NotifyProc
	(const MIDINotification *inMessage, void *inRefCon)
{
}


