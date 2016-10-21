// MIDICLOutputPort.cpp

// INCLUDES

#include "MIDICLOutputPort.h"

#include "MIDICLClient.h"
#include "MIDICLException.h"

// PUBLIC CONSTRUCTORS/DESTRUCTOR

MIDICLOutputPort::MIDICLOutputPort
	(MIDIClientRef inClientRef, CFStringRef inName)
	// throws MIDICLException
{
	OSStatus	errCode = MIDIOutputPortCreate
		(inClientRef, inName, &mPortRef);

	if (errCode != 0)
	{
		throw MIDICLException
			(MIDICLException::kMIDIOutputPortCreate, errCode);
	}
}

MIDICLOutputPort::~MIDICLOutputPort ()
{
	MIDIPortDispose (mPortRef);
}

// PUBLIC CONVENIENCE METHODS

void
MIDICLOutputPort::SendChannelAftertouch (Byte inChannel, Byte inPressure)
{
	SendSmallPacket (MIDICLClient::kMIDIChannelAftertouchMessage | inChannel, inPressure);
}

void
MIDICLOutputPort::SendPolyAftertouch (Byte inChannel, Byte inKey, Byte inPressure)
{
	SendSmallPacket (MIDICLClient::kMIDIPolyAftertouchMessage | inChannel, inKey, inPressure);
}

void
MIDICLOutputPort::SendControlChange (Byte inChannel, Byte inControl, Byte inValue)
{
	SendSmallPacket (MIDICLClient::kMIDIControlChangeMessage | inChannel, inControl, inValue);
}

void
MIDICLOutputPort::SendNoteOff (Byte inChannel, Byte inKey, Byte inVelocity)
{
	SendSmallPacket (MIDICLClient::kMIDINoteOffMessage | inChannel, inKey, inVelocity);
}

void
MIDICLOutputPort::SendNoteOn (Byte inChannel, Byte inKey, Byte inVelocity)
{
	SendSmallPacket (MIDICLClient::kMIDINoteOnMessage | inChannel, inKey, inVelocity);
}

void
MIDICLOutputPort::SendPitchBend (Byte inChannel, Byte inLSB, Byte inMSB)
{
	SendSmallPacket (MIDICLClient::kMIDIPitchBendMessage | inChannel, inLSB, inMSB);
}

void
MIDICLOutputPort::SendProgramChange (Byte inChannel, Byte inProgram)
{
	SendSmallPacket (MIDICLClient::kMIDIProgramChangeMessage | inChannel, inProgram);
}

void
MIDICLOutputPort::SendClock ()
{
	SendSmallPacket (MIDICLClient::kMIDIClockMessage);
}

void
MIDICLOutputPort::SendStart ()
{
	SendSmallPacket (MIDICLClient::kMIDIStartMessage);
}

void
MIDICLOutputPort::SendContinue ()
{
	SendSmallPacket (MIDICLClient::kMIDIContinueMessage);
}

void
MIDICLOutputPort::SendStop ()
{
	SendSmallPacket (MIDICLClient::kMIDIStopMessage);
}

// PUBLIC METHODS

void
MIDICLOutputPort::SendPacketList (const MIDIPacketList *inPacketList)
{
	OSStatus	errCode = MIDISend (mPortRef, mDestination, inPacketList);

	if (errCode != 0)
	{
		throw MIDICLException (MIDICLException::kMIDISend, errCode);
	}
}

void
MIDICLOutputPort::SendSmallPacket (Byte inOne)
{
	Byte	packetListBuffer [32];

	MIDIPacketList	*packetList ((MIDIPacketList *) packetListBuffer);
	MIDIPacket	*packet (MIDIPacketListInit (packetList));

	Byte	buffer [1];

	buffer [0] = inOne;

	packet = MIDIPacketListAdd (packetList, sizeof (packetListBuffer),
		packet, 0, 1, buffer);

	if (packet == NULL)
	{
		throw MIDICLException (MIDICLException::kMIDIPacketListAdd, 0);
	}

	OSStatus	errCode = MIDISend (mPortRef, mDestination, packetList);

	if (errCode != 0)
	{
		throw MIDICLException (MIDICLException::kMIDISend, errCode);
	}
}

void
MIDICLOutputPort::SendSmallPacket
	(Byte inOne, Byte inTwo)
{
	Byte	packetListBuffer [32];

	MIDIPacketList	*packetList ((MIDIPacketList *) packetListBuffer);
	MIDIPacket	*packet (MIDIPacketListInit (packetList));

	Byte	buffer [2];

	buffer [0] = inOne;
	buffer [1] = inTwo;

	packet = MIDIPacketListAdd (packetList, sizeof (packetListBuffer),
		packet, 0, 2, buffer);

	if (packet == NULL)
	{
		throw MIDICLException (MIDICLException::kMIDIPacketListAdd, 0);
	}

	OSStatus	errCode = MIDISend (mPortRef, mDestination, packetList);

	if (errCode != 0)
	{
		throw MIDICLException (MIDICLException::kMIDISend, errCode);
	}
}

void
MIDICLOutputPort::SendSmallPacket
	(Byte inOne, Byte inTwo, Byte inThree)
{
	Byte	packetListBuffer [32];

	MIDIPacketList	*packetList ((MIDIPacketList *) packetListBuffer);
	MIDIPacket	*packet (MIDIPacketListInit (packetList));

	Byte	buffer [3];

	buffer [0] = inOne;
	buffer [1] = inTwo;
	buffer [2] = inThree;

	packet = MIDIPacketListAdd (packetList, sizeof (packetListBuffer),
		packet, 0, 3, buffer);

	if (packet == NULL)
	{
		throw MIDICLException (MIDICLException::kMIDIPacketListAdd, 0);
	}

	OSStatus	errCode = MIDISend (mPortRef, mDestination, packetList);

	if (errCode != 0)
	{
		throw MIDICLException (MIDICLException::kMIDISend, errCode);
	}
}

void
MIDICLOutputPort::SendSysEx
	(const Byte *inBuffer, unsigned int inLength)
{
	// have to remember the original buffer
	// as the sysex send call bumps the one in the structure
	mSysExBuffer = inBuffer;

	mSysExSendRequest.destination = mDestination;
	mSysExSendRequest.data = (Byte *) mSysExBuffer;
	mSysExSendRequest.bytesToSend = inLength;
	mSysExSendRequest.complete = false;
	mSysExSendRequest.completionProc = SysExCompletionProc;
	mSysExSendRequest.completionRefCon = this;

	OSStatus	errCode = MIDISendSysex (&mSysExSendRequest);

	if (errCode != 0)
	{
		delete [] mSysExBuffer;

		throw MIDICLException (MIDICLException::kMIDISendSysEx, errCode);
	}
}

void
MIDICLOutputPort::SetDestination (MIDIEndpointRef inDestination)
{
	mDestination = inDestination;
}

// PRIVATE METHODS

void
MIDICLOutputPort::SendSysExCompleted ()
{
	delete [] mSysExBuffer;
}

// STATIC PRIVATE METHODS

void
MIDICLOutputPort::SysExCompletionProc
	(MIDISysexSendRequest *inRequest)
{
	MIDICLOutputPort	*self
		((MIDICLOutputPort *) inRequest->completionRefCon);

	self->SendSysExCompleted ();
}

