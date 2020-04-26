#ifndef MRPEventStatus_h
#define MRPEventStatus_h

// NAMESPACE

using namespace std;

// INCLUDES

#include <string>

// FORWARD DECLARATIONS

class MRPSequencer;
// class MRPSequencerSMF;
class MRPTrack;

// MRPEventStatus

class MRPEventStatus
{
	// friends
	friend class MRPSequencer;
	// friend class MRPSequencerSMF;
	friend class MRPTrack;

	// public constructor
	public:

		MRPEventStatus ();

	// public methods
	public:

		inline bool
		IsHSoloed () const
		{
			return mHSoloed;
		}

		inline bool
		IsMuted () const
		{
			return mMuted;
		}

		inline bool
		IsSelected () const
		{
			return mSelected;
		}

		inline bool
		IsVSoloed () const
		{
			return mVSoloed;
		}

	// private data members
	private:

		bool
		mIsNoteOn;

		bool
		mHSoloed;

		bool
		mMuted;

		bool
		mSelected;

		bool
		mVSoloed;

		// NOTE internal rep is 0-15, external rep is 1-16
		unsigned char
		mChannelPlaying;

		// note,control number,etc
		unsigned char
		mData1;

		unsigned char
		mNotePlaying;

		unsigned char
		mPortPlaying;
};

#endif

