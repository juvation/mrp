// MRPTrack.h

#ifndef MRPTrack_h
#define MRPTrack_h

// includes

#include "MRPEvent.h"
#include "MRPEventStatus.h"

// class

class MRPTrack
{
	// public constructors
	public:
		
		MRPTrack ();
		
	// public methods
	// step indexes are ONE based
	public:

		int
		GetHSoloCount () const;

		MRPEvent &
		GetEvent (int inStep1);

		const MRPEvent &
		GetEvent (int inStep1) const;

		MRPEventStatus &
		GetEventStatus (int inStep1);

		const MRPEventStatus &
		GetEventStatus (int inStep1) const;

		int
		GetEventCount () const
		{
			return mEventCount;
		}

		// track-mute
		// overrides step-mute
		void
		Mute ();

		// step-mute
		void
		Mute (int inStep1);
		
		void
		HSolo (int inStep1);

		void
		HUnSolo (int inStep1);
		
		bool
		IsMuted () const
		{
			return mMuted;
		}

		// track-mute
		void
		UnMute ();
		
		// step-mute
		void
		UnMute (int inStep1);
		
	// private static data members
	private:

		static const int
		kMaxSteps = 64;

	// private data members
	private:

		bool
		mMuted;

		unsigned char
		mChannel;

		int
		mHSoloCount;

		int
		mEventCount;

		MRPEvent
		mEvents [kMaxSteps];

		MRPEventStatus
		mEventStatus [kMaxSteps];
};

#endif

