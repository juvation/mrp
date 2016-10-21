// MRPSelectionAction.h

#ifndef MRPSelectionAction_h
#define MRPSelectionAction_h

// FORWARD DECLARATIONS

class MRPSequencer;

// CLASS DECLARATIONS

class MRPSelectionAction
{
	// public constructor/destructor
	public:

		virtual ~
		MRPSelectionAction ();

	// public pure virtual methods
	public:

		virtual void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1) = 0;
};

class MRPSelectionChangeChannel
	:
	public MRPSelectionAction
{
	// public constructor
	public:

		MRPSelectionChangeChannel (int inFromPort, int inFromChannel,
			int inToPort, int inToChannel);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mFromChannel;

		int
		mFromPort;

		int
		mToChannel;

		int
		mToPort;
};

class MRPSelectionChangeNote
	:
	public MRPSelectionAction
{
	// public constructor
	public:

		MRPSelectionChangeNote (unsigned char inFromNote, unsigned char inToNote);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mFromNote;

		unsigned char
		mToNote;
};

class MRPSelectionClear
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionHSolo
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionHUnSolo
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionIncrementChannel
	:
	public MRPSelectionAction
{
	// public constructor
	public:

		MRPSelectionIncrementChannel (int inDelta);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mDelta;
};

class MRPSelectionIncrementGateTime
	:
	public MRPSelectionAction
{
	// public constructor
	public:

		MRPSelectionIncrementGateTime (int inDelta);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mDelta;
};

class MRPSelectionIncrementNote
	:
	public MRPSelectionAction
{
	// public constructor
	public:

		MRPSelectionIncrementNote (int inDelta);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mDelta;
};

class MRPSelectionIncrementPort
	:
	public MRPSelectionAction
{
	// public constructor
	public:

		MRPSelectionIncrementPort (int inDelta);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mDelta;
};

class MRPSelectionIncrementVelocity
	:
	public MRPSelectionAction
{
	// public constructor
	public:

		MRPSelectionIncrementVelocity (int inDelta);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mDelta;
};

class MRPSelectionInvertMute
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionMute
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionRandomiseChannel
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionRandomiseChannel (int inBase, int inRange);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mBase;

		int
		mRange;
};

class MRPSelectionRandomiseData1
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionRandomiseData1 (int inBase, int inRange);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mBase;

		int
		mRange;
};

class MRPSelectionRandomiseData2
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionRandomiseData2 (int inBase, int inRange);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mBase;

		int
		mRange;
};

class MRPSelectionRandomiseGate
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionRandomiseGate (int inBase, int inRange);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mBase;

		int
		mRange;
};

class MRPSelectionRandomMute
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionRandomiseNote
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionRandomiseNote (int inBase, int inRange);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mBase;

		int
		mRange;
};

class MRPSelectionRandomisePort
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionRandomisePort (int inBase, int inRange);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mBase;

		int
		mRange;
};

class MRPSelectionRandomiseVelocity
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionRandomiseVelocity (int inBase, int inRange);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		int
		mBase;

		int
		mRange;
};

class MRPSelectionSelectByClockDivision
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSelectByClockDivision (int inDivision);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	private:

		int
		mDivision;
};

class MRPSelectionSetChannel
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSetChannel (unsigned char inChannel);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mChannel;
};

class MRPSelectionSetData1
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSetData1 (unsigned char inData1);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mData1;
};

class MRPSelectionSetData2
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSetData2 (unsigned char inData2);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mData2;
};

class MRPSelectionSetPort
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSetPort (unsigned char inPort);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mPort;
};

class MRPSelectionSetNote
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSetNote (unsigned char inNote);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mNote;
};

class MRPSelectionSetGateTime
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSetGateTime (unsigned char inGateTime);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mGateTime;
};

class MRPSelectionSetType
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSetType (unsigned char inCommandByte);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mCommandByte;
};

class MRPSelectionSetVelocity
	:
	public MRPSelectionAction
{
	// public constructor/destructor
	public:

		MRPSelectionSetVelocity (unsigned char inVelocity);
		
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private data members
	private:

		unsigned char
		mVelocity;
};

class MRPSelectionUnMute
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionUnSelect
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionVSolo
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

class MRPSelectionVUnSolo
	:
	public MRPSelectionAction
{
	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);
};

#endif	// MRPSelectionAction_h

