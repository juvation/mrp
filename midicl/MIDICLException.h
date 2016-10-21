// MIDICLException.h

// GUARD

#ifndef MIDICLException_h
#define MIDICLException_h

// CLASS

class MIDICLException
{
	// static public constants
	public:

		typedef enum
		{
			kMIDIClientCreate = 1,
			kMIDIDestinationCreate = 2,
			kMIDIInputPortCreate = 3,
			kMIDIOutputPortCreate = 4,
			kMIDIPacketListAdd = 5,
			kMIDISend = 6,
			kMIDISendSysEx = 7,
		} MIDICLCallNumber;

	// public constructors/destructor
	public:

		MIDICLException
			(MIDICLCallNumber inCallNumber, OSStatus inErrCode)
			:
			mCallNumber (inCallNumber),
			mErrCode (inErrCode)
		{
		}

	// public methods
	public:

		MIDICLCallNumber
		GetCallNumber () const
		{
			return mCallNumber;
		}

		OSStatus
		GetErrCode () const
		{
			return mErrCode;
		}

	// private data
	private:

		MIDICLCallNumber
		mCallNumber;

		OSStatus
		mErrCode;
};

#endif	// MIDICLException_h

