// MIDICLOutputPort.h

// GUARD

#ifndef MIDICLOutputPort_h
#define MIDICLOutputPort_h

// INCLUDES

#include <CoreMIDI/MIDIServices.h>

// CLASS

class MIDICLOutputPort
{
	// public constructors/destructor
	public:

		MIDICLOutputPort (MIDIClientRef inClientRef, CFStringRef inName);
			// throws MIDICLException

		~MIDICLOutputPort ();

	// public convenience methods
	public:

		void
		SendChannelAftertouch (Byte inChannel, Byte inPressure);

		void
		SendPolyAftertouch (Byte inChannel, Byte inKey, Byte inPressure);

		void
		SendControlChange (Byte inChannel, Byte inControl, Byte inValue);

		void
		SendNoteOff (Byte inChannel, Byte inKey, Byte inVelocity);

		void
		SendNoteOn (Byte inChannel, Byte inKey, Byte inVelocity);

		void
		SendPitchBend (Byte inChannel, Byte inLSB, Byte inMSB);

		void
		SendProgramChange (Byte inChannel, Byte inProgram);

		void
		SendClock ();

		void
		SendStart ();

		void
		SendContinue ();

		void
		SendStop ();

	// public byte-level methods
	public:

		void
		SendPacketList (const MIDIPacketList *inPacketList);
			// throws MIDICLException

		void
		SendSmallPacket (Byte inOne);
			// throws MIDICLException

		void
		SendSmallPacket (Byte inOne, Byte inTwo);
			// throws MIDICLException

		void
		SendSmallPacket (Byte inOne, Byte inTwo, Byte inThree);
			// throws MIDICLException

		// takes ownership of the buffer
		// frees buffer on error or completion
		void
		SendSysEx (const Byte *inBuffer, unsigned int inLength);
			// throws MIDICLException

		void
		SendSysExCompleted ();

		void
		SetDestination (MIDIEndpointRef inDestination);
			// throws MIDICLException

	// static private methods
	private:

		static void
		SysExCompletionProc (MIDISysexSendRequest *inRequest);

	// private constructors
	private:

		MIDICLOutputPort (const MIDICLOutputPort &inCopy);

	// private operators overloaded
	private:

		MIDICLOutputPort &
		operator = (const MIDICLOutputPort &inCopy);

	// private data
	private:

		const Byte *
		mSysExBuffer;

		MIDIEndpointRef
		mDestination;

		MIDIPortRef
		mPortRef;

		MIDISysexSendRequest
		mSysExSendRequest;

};

#endif	// MIDICLOutputPort_h


