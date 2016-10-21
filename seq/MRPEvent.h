#ifndef MRPEvent_h
#define MRPEvent_h

// NAMESPACE

using namespace std;

// INCLUDES

#include <string>

// FORWARD DECLARATIONS

class MRPSequencer;
class MRPTrack;

// MRPEVENT

class MRPEvent
{
	// friends
	friend class MRPSequencer;
	friend class MRPSequencerSMF;
	friend class MRPTrack;

	// static methods
	public:		

		static string
		FormatNoteNumber (unsigned char inNote);

		static int
		ParseNoteNumber (string inNoteName);

	// public constructor
	public:

		MRPEvent ();

	// public methods
	public:

		// NOTE internal rep is 0-15, external rep is 1-16
		inline unsigned char
		GetChannel () const
		{
			return mChannel + 1;
		}

		inline unsigned char
		GetGateTime () const
		{
			return mGateTime;
		}

		inline unsigned char
		GetData1 () const
		{
			return mData1;
		}

		inline unsigned char
		GetData2 () const
		{
			return mData2;
		}

		inline unsigned char
		GetType () const
		{
			return mMessage;
		}

		inline unsigned char
		GetPort () const
		{
			return mPort;
		}

		inline unsigned char
		GetMessage () const
		{
			return mMessage;
		}

	// static private data members
	private:

		static const char *
		sNoteNames [];

	// private data members
	private:

		// NOTE this is 0x80 - 0xd0
		// for noteoff,noteon,polypressure,
		// control,program,channelpressure,pitchbend
		unsigned char
		mMessage;

		// NOTE internal rep is 0-15, external rep is 1-16
		unsigned char
		mChannel;

		unsigned char
		mGateTime;

		// note,control number,etc
		unsigned char
		mData1;

		// velocity,control value, etc
		unsigned char
		mData2;

		unsigned char
		mPort;
};

#endif

