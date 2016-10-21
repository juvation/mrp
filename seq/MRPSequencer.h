// MRPSequencer.h

#ifndef MRPSEQUENCER_H
#define MRPSEQUENCER_H

// NAMESPACES

using namespace std;

// INCLUDES

#include <CoreServices/CoreServices.h>

#include <map>
#include <string>
#include <vector>

#include "MIDICLClient.h"
#include "MIDICLDestination.h"

#include "MRPSequencerClock.h"
#include "MRPTrack.h"

// FORWARD DECLARATIONS

class MIDICLInputPort;
class MIDICLOutputPort;

class MRPSelectionAction;
class MRPSequencerListener;

// CLASS

class MRPSequencer
{
	// public constructors/destructors
	public:

		MRPSequencer ();

		virtual ~
		MRPSequencer ();

	// public methods
	// step & track numbers are ONE based
	public:

		int
		AddListener (MRPSequencerListener *inListener);

		void
		RemoveListener (MRPSequencerListener *inListener);

		int
		AddInputPort (MIDICLInputPort *inInputPort);

		int
		AddOutputPort (MIDICLOutputPort *inOutputPort);

		int
		AddTrack (const MRPTrack &inTrackObject);

		int
		AddTrack (const MRPTrack &inTrackObject, int inTrack1);

		void
		CopyTrack (int inFromTrack, int inToTrack);

		void
		DeleteChannelNames ();

		void
		DeleteTrack (int inTrack1);

		int
		GetBarCount () const;

		double
		GetBPM ();

		string
		GetChannelName (int inPort, int inChannel);

		int
		GetCurrentStep () const;

		// 1-based
		MRPEvent &
		GetEvent (int inTrack1, int inStep1);

		// 1-based
		const MRPEvent &
		GetEvent (int inTrack1, int inStep1) const;

		// 1-based
		MRPEventStatus &
		GetEventStatus (int inTrack1, int inStep1);

		// 1-based
		const MRPEventStatus &
		GetEventStatus (int inTrack1, int inStep1) const;

		// 1-based
		int
		GetLoopStep () const;

		bool
		GetNamedChannel (string inName, int *outPort, int *outChannel);

		int
		GetInputPortCount () const;

		int
		GetOutputPortCount () const;

		int
		GetSelectedCount () const;

		int
		GetStepCount () const;

		int
		GetTicksPerQuarterNote () const
		{
			return kTicksPerQuarterNote;
		}

		int
		GetTicksPerStep () const
		{
			return mTicksPerStep;
		}

		MRPTrack &
		GetTrack (int inTrack1);

		const MRPTrack &
		GetTrack (int inTrack1) const;

		int
		GetTrackCount () const;

		string
		GetTrackName (int inTrack1) const;

		int
		GetTrackNumber (string inTrackName) const;

		void
		HSolo (int inTrack1, int inStep1);

		void
		HUnSolo (int inTrack1, int inStep1);

		void
		InvertMute (int inTrack1, int inStep1);

		void
		InvertSelect (int inTrack1, int inStep1);

		bool
		IsPlaying () const
		{
			return mClock->IsStarted ();
		}

		// step-mute
		void
		Mute (int inTrack1, int inStep1);

		// track-mute
		void
		MuteTrack (int inTrack1);

		void
		Play ();

		void
		Reset ();

		int
		RotateSelectionForwards (int inRotateDelta);

		int
		RotateSelectionBackwards (int inRotateDelta);

		int
		RunSelection (MRPSelectionAction *inAction);

		int
		SelectAll ();

		int
		SelectEvent (int inTrack1, int inStep1);

		int
		SelectStep (int inStep1);

		int
		SelectTrack (int inTrack1);

		void
		SendAllNotesOff ();

		void
		SendMessage (MRPEvent *ioEvent, MRPEventStatus *ioEventStatus);

		void
		SendNoteOn (MRPEvent *ioEvent, MRPEventStatus *ioEventStatus);

		void
		SendNoteOff (MRPEvent *ioEvent, MRPEventStatus *ioEventStatus);

		void
		SetBPM (double inBPM);

		void
		SetChannel (int inTrack1, int inStep1, int inChannel);

		void
		SetChannelName (int inPort, int inChannel, string inName);

		void
		SetData1 (int inTrack1, int inStep1, int inData1);

		void
		SetData2 (int inTrack1, int inStep1, int inData2);

		void
		SetEvent (int inTrack1, int inStep1, const MRPEvent &inEvent);

		void
		SetGateTime (int inTrack1, int inStep1, int inGate);

		void
		SetInternalSync ();

		void
		SetLoopStep (int inStep1);

		void
		SetMIDISync (int inPortNumber);

		void
		SetMute (int inTrack1, int inStep1, bool inMute);

		void
		SetNote (int inTrack1, int inStep1, int inNote);

		void
		SetPort (int inTrack1, int inStep1, int inPort);

		void
		SetScale (int inBars, int inStep1s);

		void
		SetTrackName (int inTrack1, string inName);

		void
		SetType (int inTrack1, int inStep1, unsigned char inCommandByte);

		void
		SetVelocity (int inTrack1, int inStep1, int inVelocity);

		void
		Stop ();

		void
		TimerTick ();

		// step-mute
		void
		UnMute (int inTrack1, int inStep1);

		// track-mute
		void
		UnMuteTrack (int inTrack1);

		int
		UnSelectEvent (int inTrack1, int inStep1);

		int
		UnSelectStep (int inStep1);

		int
		UnSelectTrack (int inTrack1);

		int
		VarySelection ();

		int
		VarySelection2 ();

		void
		VSolo (int inTrack1, int inStep1);

		void
		VUnSolo (int inTrack1, int inStep1);

	// protected static methods
	protected:

		static void
		TimeManagerTask (TMTaskPtr inTaskPtr);

	// protected action-firing methods
	// step & track numbers are ZERO based
	protected:

		void
		FireBPMChangedAction ();

		void
		FireClockResetAction ();

		void
		FireConfigChangedAction ();

		void
		FireEventChangedAction (int inTrack0, int inStep0);

		void
		FireLoopChangedAction ();

		void
		FirePauseAction ();

		void
		FirePortListChangedAction ();

		void
		FireScaleChangedAction ();

		void
		FireStartAction ();

		void
		FireStepTickAction ();

		void
		FireStepTickPostAction ();

		void
		FireStopAction ();

		void
		FireTimerTickAction ();

		void
		FireTimerTickPostAction ();

		void
		FireTrackChangedAction (int inTrack0);

		void
		FireTrackAddedAction (int inTrack0);

		void
		FireTrackDeletedAction (int inTrack0);

	// protected methods
	protected:

		// 0-based
		MRPEvent &
		GetEvent0 (int inTrack0, int inStep0);

		// 0-based
		const MRPEvent &
		GetEvent0 (int inTrack0, int inStep0) const;

		// 0-based
		MRPEventStatus &
		GetEventStatus0 (int inTrack0, int inStep0);

		// 0-based
		const MRPEventStatus &
		GetEventStatus0 (int inTrack0, int inStep0) const;

		int
		GetGateTicks (int inGatePercent);

		void
		SetEvent0 (int inTrack0, int inStep0, const MRPEvent &inEvent);

	// private static data members
	private:

		static const int
		kMaxSteps = 64;

		static const int
		kTicksPerQuarterNote = 24;

	// private data members
	private:
		
		MIDICLDestination *
		mDummyDestination;

		MRPSequencerClock *
		mClock;

		bool
		mSequencerIsBusy;

		bool
		mVSoloed [kMaxSteps];

		int
		mTicks;

		int
		mTicksInLoop;

		int
		mTicksInSequence;

		int
		mTicksPerStep;

		unsigned long
		mTotalTicks;

		double
		mBPM;

		MIDICLClient
		mMIDIClient;

		map <string, string>
		mChannelNames;

		map <string, string>
		mNamedChannels;

		// owning list
		vector <MIDICLInputPort *>
		mInputPorts;

		// owning list
		vector <MIDICLOutputPort *>
		mOutputPorts;

		// non-owning list
		vector <MRPSequencerListener *>
		mListeners;

		vector <string>
		mTrackNames;
	
		vector <MRPTrack>
		mTracks;
	
};

#endif	// MRPSEQUENCER_H

