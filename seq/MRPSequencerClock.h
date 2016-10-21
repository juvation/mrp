#ifndef MRPSequencerClock_h
#define MRPSequencerClock_h

// NAMESPACE

using namespace std;

// INCLUDES

// FORWARD DECLARATIONS

class MRPSequencer;

// MRPSequencerClock

class MRPSequencerClock
{
	// public constructor
	public:

		MRPSequencerClock (MRPSequencer *inSequencer);

		virtual ~
		MRPSequencerClock ();

	// public pure virtual methods
	public:

		virtual double
		GetBPM () const = 0;

		virtual int
		SetBPM (double inBPM) = 0;

		virtual int
		Start () = 0;

		virtual void
		Stop () = 0;

	// public methods
	public:

		bool
		IsStarted () const
		{
			return mStarted;
		}

	// protected data
	protected:

		MRPSequencer *
		mSequencer;

		bool
		mStarted;
};

#endif	// MRPSequencerClock_h


