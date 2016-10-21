// MRPSequencer.cpp

// MRP includes

#include "MRPSequencer.h"

#include "MRPMIDIClock.h"
#include "MRPSelectionAction.h"
#include "MRPSequencerListener.h"
#include "MRPTimeManagerClock.h"

// MIDI includes

#include "MIDICLClient.h"
#include "MIDICLInputPort.h"
#include "MIDICLOutputPort.h"

// STL

#include <iterator>
#include <vector>

// NAMESPACE

using namespace std;

// PUBLIC CONSTRUCTORS

MRPSequencer::MRPSequencer ()
	:
	mDummyDestination (NULL),
	mClock (NULL),
	mSequencerIsBusy (false),
	mTicks (-1),
	mTicksInLoop (96),
	mTicksInSequence (96),
	mTicksPerStep (6),
	mTotalTicks (0),
	mMIDIClient (CFSTR ("MRPSequencer"))
{
	// default to internal clock
	mClock = new MRPTimeManagerClock (this);

	for (int s = 0; s < kMaxSteps; s++)
	{
		mVSoloed [s] = false;
	}
	
	// open input & output ports
	int	numSources = MIDIGetNumberOfSources ();
	int	numDestinations = MIDIGetNumberOfDestinations ();

	if (numDestinations == 0)
	{
		mDummyDestination = mMIDIClient.MakeDestination
			(CFSTR ("MRPSequencer dummy destination"));
	}

	for (int i = 0; i < numSources; i++)
	{
		MIDICLInputPort	*inputPort (mMIDIClient.MakeInputPort (CFSTR ("input")));
		inputPort->SetSource (MIDIGetSource (i));

		AddInputPort (inputPort);
	}

	for (int i = 0; i < numDestinations; i++)
	{
		MIDICLOutputPort	*outputPort (mMIDIClient.MakeOutputPort (CFSTR ("output")));
		outputPort->SetDestination (MIDIGetDestination (i));

		AddOutputPort (outputPort);
	}
}

MRPSequencer::~MRPSequencer ()
{
	for (int i = 0; i < (int) mInputPorts.size (); i++)
	{
		MIDICLInputPort	*inputPort (mInputPorts [i]);

		mInputPorts [i] = NULL;

		delete inputPort;
	}

	for (int i = 0; i < (int) mOutputPorts.size (); i++)
	{
		MIDICLOutputPort	*outputPort (mOutputPorts [i]);

		mOutputPorts [i] = NULL;

		delete outputPort;
	}

	if (mDummyDestination != NULL)
	{
		delete mDummyDestination;
	}
}

// PUBLIC METHODS

int
MRPSequencer::AddInputPort (MIDICLInputPort *inInputPort)
{
	mInputPorts.push_back (inInputPort);

	return mInputPorts.size () - 1;
}

int
MRPSequencer::AddOutputPort (MIDICLOutputPort *inOutputPort)
{
	mOutputPorts.push_back (inOutputPort);

	return mOutputPorts.size () - 1;
}

int
MRPSequencer::AddListener (MRPSequencerListener *inListener)
{
	mListeners.push_back (inListener);

	return mListeners.size () - 1;
}

void
MRPSequencer::RemoveListener (MRPSequencerListener *inListener)
{
	vector <MRPSequencerListener *>::iterator found (mListeners.end ());
	vector <MRPSequencerListener *>::iterator it;

	for (it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		if (*it == inListener)
		{
			found = it;
		}
	}

	if (found != mListeners.end ())
	{
		mListeners.erase (found);
	}
}

int
MRPSequencer::AddTrack (const MRPTrack &inTrackObject)
{
	while (mSequencerIsBusy);

	mTracks.push_back (inTrackObject);

	// 0-based
	FireTrackAddedAction (mTracks.size () - 1);

	// 1-based
	return mTracks.size ();
}

int
MRPSequencer::AddTrack (const MRPTrack &inTrackObject, int inTrack1)
{
	int	track0 (inTrack1 - 1);

	while (mSequencerIsBusy);

	if (track0 == GetTrackCount ())
	{
		mTracks.push_back (inTrackObject);
	}
	else if (track0 > GetTrackCount ())
	{
		int	oldTrackCount (GetTrackCount ());

		// make padding tracks
		mTracks.resize (track0 + 1);

		// tell the listeners about the "padding" tracks only
		for (int t = oldTrackCount; t < track0; t++)
		{
			// 0-based
			FireTrackAddedAction (t);
		}

		mTracks.push_back (inTrackObject);
	}
	else
	{
		// insert the track at the right spot
		vector <MRPTrack>::iterator	insertion (mTracks.begin ());

		for (int t = 0; t < track0; t++)
		{
			insertion++;
		}

		mTracks.insert (insertion, inTrackObject);
	}

	// 0-based
	FireTrackAddedAction (track0);

	// 1-based
	return inTrack1;
}

void
MRPSequencer::CopyTrack (int inFromTrack1, int inToTrack1)
{
	GetTrack (inToTrack1) = GetTrack (inFromTrack1);

	// 0-based
	FireTrackChangedAction (inToTrack1 - 1);
}

void
MRPSequencer::DeleteChannelNames ()
{
	mChannelNames.clear ();
	mNamedChannels.clear ();

	FireConfigChangedAction ();
}

void
MRPSequencer::DeleteTrack (int inTrack1)
{
	while (mSequencerIsBusy);

	mSequencerIsBusy = true;

	int	track0 (inTrack1 - 1);

	if (track0 < GetTrackCount ())
	{
		// 1-based
		MuteTrack (inTrack1);

		// mute any playing notes
		for (int s = 0; s < GetStepCount (); s++)
		{
			MRPEvent	&event (GetEvent0 (track0, s));
			MRPEventStatus	&status (GetEventStatus0 (track0, s));

			if (status.mIsNoteOn)
			{
				SendNoteOff (&event, &status);
			}
		}

		vector <MRPTrack>::iterator	deletion (mTracks.begin ());

		for (int t = 0; t < track0; t++)
		{
			deletion++;
		}

		mTracks.erase (deletion);

		// 0-based
		FireTrackDeletedAction (track0);
	}

	mSequencerIsBusy = false;
}

int
MRPSequencer::GetBarCount () const
{
	return mTicksInSequence / kTicksPerQuarterNote / 4;
}

double
MRPSequencer::GetBPM ()
{
	double	bpm (mClock->GetBPM ());

	if (bpm != 0.0)
	{
		mBPM = bpm;
	}

	return bpm;
}

string
MRPSequencer::GetChannelName (int inPort, int inChannel)
{
	string	name;
	char	buffer [16];

	sprintf (buffer, "%d %d", inPort, inChannel);

	map <string, string>::iterator	found (mChannelNames.find (buffer));

	if (found != mChannelNames.end ())
	{
		name = mChannelNames [buffer];
	}

	return name;
}

int
MRPSequencer::GetCurrentStep () const
{
	return mTicks / mTicksPerStep;
}

MRPEvent &
MRPSequencer::GetEvent (int inTrack1, int inStep1)
{
	return GetTrack (inTrack1).GetEvent (inStep1);
}

const MRPEvent &
MRPSequencer::GetEvent (int inTrack1, int inStep1) const
{
	return GetTrack (inTrack1).GetEvent (inStep1);
}

MRPEventStatus &
MRPSequencer::GetEventStatus (int inTrack1, int inStep1)
{
	return GetTrack (inTrack1).GetEventStatus (inStep1);
}

const MRPEventStatus &
MRPSequencer::GetEventStatus (int inTrack1, int inStep1) const
{
	return GetTrack (inTrack1).GetEventStatus (inStep1);
}

int
MRPSequencer::GetInputPortCount () const
{
	return mInputPorts.size ();
}

// 1-based
int
MRPSequencer::GetLoopStep () const
{
	return (mTicksInLoop / mTicksPerStep) + 1;
}

bool
MRPSequencer::GetNamedChannel (string inName, int *outPort, int *outChannel)
{
	map <string, string>::iterator	found (mNamedChannels.find (inName));

	if (found != mNamedChannels.end ())
	{
		// grab it
		string	buffer (mNamedChannels [inName]);

		sscanf (buffer.c_str (), "%d %d", outPort, outChannel);
	}

	return (found != mNamedChannels.end ());
}

int
MRPSequencer::GetOutputPortCount () const
{
	return mOutputPorts.size ();
}

int
MRPSequencer::GetSelectedCount () const
{
	int	selected (0);
	int	stepCount (GetStepCount ());

	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < stepCount; s++)
		{
			if (GetEventStatus0 (t, s).IsSelected ())
			{
				selected++;
			}
		}
	}

	return selected;
}

int
MRPSequencer::GetStepCount () const
{
	return mTicksInSequence / mTicksPerStep;
}

MRPTrack &
MRPSequencer::GetTrack (int inTrack1)
{
	int	track0 (inTrack1 - 1);

	return mTracks [track0];
}

const MRPTrack &
MRPSequencer::GetTrack (int inTrack1) const
{
	int	track0 (inTrack1 - 1);

	return mTracks [track0];
}

int
MRPSequencer::GetTrackCount () const
{
	return mTracks.size ();
}

string
MRPSequencer::GetTrackName (int inTrack1) const
{
	int	track0 (inTrack1 - 1);
	string	name;

	if (track0 >= 0 && track0 < (int) mTrackNames.size ())
	{
		name = mTrackNames [track0];
	}

	return name;
}

int
MRPSequencer::GetTrackNumber (string inTrackName) const
{
	int	trackNumber1 = -1;

	for (int track0 = 0; track0 < (int) mTrackNames.size (); track0++)
	{
		if (strcasecmp (mTrackNames [track0].c_str (), inTrackName.c_str ()) == 0)
		{
			trackNumber1 = track0 + 1;
			break;
		}
	}

	return trackNumber1;
}

void
MRPSequencer::MuteTrack (int inTrack1)
{
	GetTrack (inTrack1).Mute ();

	int	track0 (inTrack1 - 1);

	// 0-based
	FireTrackChangedAction (track0);
}

void
MRPSequencer::Mute (int inTrack1, int inStep1)
{
	GetTrack (inTrack1).Mute (inStep1);

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	// 0-based
	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::HSolo (int inTrack1, int inStep1)
{
	GetTrack (inTrack1).HSolo (inStep1);

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	// 0-based
	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::HUnSolo (int inTrack1, int inStep1)
{
	GetTrack (inTrack1).HUnSolo (inStep1);

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	// 0-based
	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::InvertMute (int inTrack1, int inStep1)
{
	GetEventStatus (inTrack1, inStep1).mMuted
		= ! GetEventStatus (inTrack1, inStep1).mMuted;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::InvertSelect (int inTrack1, int inStep1)
{
	GetEventStatus (inTrack1, inStep1).mSelected
		= ! GetEventStatus (inTrack1, inStep1).mSelected;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::Play ()
{
	if (! mClock->IsStarted ())
	{
		Reset ();
		mClock->Start ();

		FireStartAction ();
	}
}

void
MRPSequencer::Reset ()
{
	if (! mClock->IsStarted ())
	{
		mTicks = -1;
		mTotalTicks = 0;
	}
}

int
MRPSequencer::RotateSelectionForwards (int inRotateCount)
{
	vector	<MRPEvent>	events;

	if (inRotateCount < 0)
	{
		return RotateSelectionBackwards (inRotateCount);
	}

	int	selectionCount (0);

	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < GetStepCount (); s++)
		{
			MRPEvent	&event (GetEvent0 (t, s));
			MRPEventStatus	&status (GetEventStatus0 (t, s));

			if (status.IsSelected ())
			{
				events.push_back (event);

				if (selectionCount >= inRotateCount)
				{
					SetEvent0 (t, s, events [0]);

					events.erase (events.begin ());
				}

				selectionCount++;
			}
		}
	}

	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < GetStepCount (); s++)
		{
			if (events.empty ())
			{
				break;
			}

			MRPEventStatus	&status (GetEventStatus0 (t, s));

			if (status.IsSelected ())
			{
				SetEvent0 (t, s, events [0]);

				events.erase (events.begin ());
			}
		}
	}

	return selectionCount;
}

int
MRPSequencer::RotateSelectionBackwards (int inRotateCount)
{
	vector	<MRPEvent>	events;

	if (inRotateCount >= 0)
	{
		return RotateSelectionForwards (inRotateCount);
	}

	// make the rotate count positive
	inRotateCount = 0 - inRotateCount;

	int	selectionCount (0);

	for (int t = GetTrackCount () - 1; t >= 0; t--)
	{
		for (int s = GetStepCount () - 1; s >= 0; s--)
		{
			MRPEvent	&event (GetEvent0 (t, s));
			MRPEventStatus	&status (GetEventStatus0 (t, s));

			if (status.IsSelected ())
			{
				events.push_back (event);

				if (selectionCount >= inRotateCount)
				{
					SetEvent0 (t, s, events [0]);

					events.erase (events.begin ());
				}

				selectionCount++;
			}
		}
	}

	for (int t = GetTrackCount () - 1; t >= 0; t--)
	{
		for (int s = GetStepCount () - 1; s >= 0; s--)
		{
			if (events.empty ())
			{
				break;
			}

			MRPEventStatus	&status (GetEventStatus0 (t, s));

			if (status.IsSelected ())
			{
				SetEvent0 (t, s, events [0]);

				events.erase (events.begin ());
			}
		}
	}

	return selectionCount;
}

int
MRPSequencer::RunSelection (MRPSelectionAction *inAction)
{
	int	numSelections = 0;

	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < GetStepCount (); s++)
		{
			if (GetEventStatus0 (t, s).mSelected)
			{
				numSelections++;

				int	track1 (t + 1);
				int	step1 (s + 1);

				// 1-based
				inAction->Action (this, track1, step1);
			}
		}
	}

	return numSelections;
}

int
MRPSequencer::SelectAll ()
{
	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < GetStepCount (); s++)
		{
			GetEventStatus0 (t, s).mSelected = true;

			FireEventChangedAction (t, s);
		}
	}

	return GetTrackCount () * GetStepCount ();
}

int
MRPSequencer::SelectEvent (int inTrack1, int inStep1)
{
	int	selections = 0;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	if (track0 < 0 || track0 >= (int) mTracks.size ())
	{
		// bad track
	}
	else
	if (step0 < 0 || step0 >= GetStepCount ())
	{
		// bad step
	}
	else
	{
		GetEventStatus0 (track0, step0).mSelected = true;

		selections = 1;

		// 0-based
		FireEventChangedAction (track0, step0);
	}

	return selections;
}

int
MRPSequencer::SelectStep (int inStep1)
{
	int	selections = 0;

	int	step0 (inStep1 - 1);

	if (step0 < 0 || step0 >= GetStepCount ())
	{
		// bad step
	}
	else
	{
		int	step0 (inStep1 - 1);

		for (int t = 0; t < GetTrackCount (); t++, selections++)
		{
			GetEventStatus0 (t, step0).mSelected = true;

			// 0-based
			FireEventChangedAction (t, step0);
		}
	}

	return selections;
}

int
MRPSequencer::SelectTrack (int inTrack1)
{
	int	selections = -1;
	int	track0 (inTrack1 - 1);

	if (track0 < 0 || track0 >= GetTrackCount ())
	{
		// bad track
	}
	else
	{
		for (int s = 0; s < GetStepCount (); s++, selections++)
		{
			GetEventStatus0 (track0, s).mSelected = true;

			// 0-based
			FireEventChangedAction (track0, s);
		}
	}

	return selections;
}

void
MRPSequencer::SendAllNotesOff ()
{
	while (mSequencerIsBusy);

	int	stepCount (GetStepCount ());

	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < stepCount; s++)
		{
			MRPEventStatus	&status (GetEventStatus0 (t, s));

			if (status.mIsNoteOn)
			{
				MRPEvent	&event (GetEvent0 (t, s));

				SendNoteOff (&event, &status);
				
				break;
			}
		}
	}
}

void
MRPSequencer::SendMessage (MRPEvent *ioEvent, MRPEventStatus *ioStatus)
{
	if (ioEvent->mMessage == MIDICLClient::kMIDINoteOnMessage)
	{
		// check velocity and gate time
		if (ioEvent->mData2 > 0 && ioEvent->mGateTime > 0)
		{
			SendNoteOn (ioEvent, ioStatus);
		}
	}
	else
	{
		MIDICLOutputPort	*outputPort = mOutputPorts [ioEvent->mPort];

		if (outputPort != NULL)
		{
			if (ioEvent->mMessage == MIDICLClient::kMIDIProgramChangeMessage
				|| ioEvent->mMessage == MIDICLClient::kMIDIChannelAftertouchMessage)
			{
				outputPort->SendSmallPacket
					(ioEvent->mMessage | ioEvent->mChannel, ioEvent->mData1);
			}
			else
			{
				outputPort->SendSmallPacket
					(ioEvent->mMessage | ioEvent->mChannel, ioEvent->mData1, ioEvent->mData2);
			}
		}
	}
}

void
MRPSequencer::SendNoteOn (MRPEvent *ioEvent, MRPEventStatus *ioStatus)
{
	MIDICLOutputPort	*outputPort = mOutputPorts [ioEvent->mPort];

	if (outputPort != NULL)
	{
		outputPort->SendNoteOn
			(ioEvent->mChannel, ioEvent->mData1, ioEvent->mData2);
	}

	// save the note number we are playing
	// and send the note off on this note
	// to guard against note changes in between on and off
	ioStatus->mNotePlaying = ioEvent->mData1;
	ioStatus->mChannelPlaying = ioEvent->mChannel;
	ioStatus->mPortPlaying = ioEvent->mPort;

	ioStatus->mIsNoteOn = true;
}

void
MRPSequencer::SendNoteOff (MRPEvent *ioEvent, MRPEventStatus *ioStatus)
{
	MIDICLOutputPort	*outputPort = mOutputPorts [ioStatus->mPortPlaying];

	if (outputPort != NULL)
	{
		outputPort->SendNoteOff
			(ioStatus->mChannelPlaying, ioStatus->mNotePlaying, 0);

	}

	ioStatus->mIsNoteOn = false;
}

void
MRPSequencer::SetBPM (double inBPM)
{
	// we are the repository
	// have to reset bpm when we change clocks
	mBPM = inBPM;

	// this might be rejected 
	// under external clock control
	mClock->SetBPM (inBPM);

	FireBPMChangedAction ();
}

void
MRPSequencer::SetChannel (int inTrack1, int inStep1, int inChannel)
{
	if (inChannel >= 1 && inChannel <= 16)
	{
		GetEvent (inTrack1, inStep1).mChannel = inChannel - 1;

		int	track0 (inTrack1 - 1);
		int	step0 (inStep1 - 1);

		FireEventChangedAction (track0, step0);
	}
}

void
MRPSequencer::SetChannelName (int inPort, int inChannel, string inName)
{
	char	buffer [16];

	sprintf (buffer, "%d %d", inPort, inChannel);

	mChannelNames [buffer] = inName;
	mNamedChannels [inName] = buffer;

	FireConfigChangedAction ();
}

void
MRPSequencer::SetEvent (int inTrack1, int inStep1, const MRPEvent &inEvent)
{
	// MRPEvent's copy-constructor doesn't copy solo or running status etc
	GetEvent (inTrack1, inStep1) = inEvent;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::SetData1 (int inTrack1, int inStep1, int inData1)
{
	GetEvent (inTrack1, inStep1).mData1
		= inData1 > 127 ? 127 : inData1;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::SetData2 (int inTrack1, int inStep1, int inData2)
{
	GetEvent (inTrack1, inStep1).mData2
		= inData2 > 127 ? 127 : inData2;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::SetGateTime (int inTrack1, int inStep1, int inGateTime)
{
	GetEvent (inTrack1, inStep1).mGateTime
		= inGateTime > 90 ? 90 : inGateTime;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::SetInternalSync ()
{
	delete mClock;

	mClock = new MRPTimeManagerClock (this);
	mClock->SetBPM (mBPM);
}

void
MRPSequencer::SetLoopStep (int inStep1)
{
	int	step0 (inStep1 - 1);

	if (step0 >= 0 && step0 <= GetStepCount ())
	{
		mTicksInLoop = step0 * mTicksPerStep;
	}
	
	FireLoopChangedAction ();
}

void
MRPSequencer::SetMIDISync (int inPortNumber)
{
	if (inPortNumber >= 0 && inPortNumber < GetInputPortCount ())
	{
		delete mClock;

		mClock = new MRPMIDIClock (this, mInputPorts [inPortNumber]);
	}
}

void
MRPSequencer::SetMute (int inTrack1, int inStep1, bool inMute)
{
	GetEventStatus (inTrack1, inStep1).mMuted = inMute;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::SetNote (int inTrack1, int inStep1, int inNote)
{
	GetEvent (inTrack1, inStep1).mData1 = inNote > 127 ? 127 : inNote;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::SetPort (int inTrack1, int inStep1, int inPort)
{
	if (inPort >= 0 && inPort < GetOutputPortCount ())
	{
		GetEvent (inTrack1, inStep1).mPort = inPort;

		int	track0 (inTrack1 - 1);
		int	step0 (inStep1 - 1);

		FireEventChangedAction (track0, step0);
	}
}

void
MRPSequencer::SetTrackName (int inTrack1, string inName)
{
	int	track0 (inTrack1 - 1);

	if (track0 >= (int) mTrackNames.size ())
	{
		mTrackNames.resize (track0 + 1);
	}

	mTrackNames [track0] = inName;
}

void
MRPSequencer::SetType (int inTrack1, int inStep1, unsigned char inCommandByte)
{
	GetEvent (inTrack1, inStep1).mMessage
		= inCommandByte & 0xf0;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::SetVelocity (int inTrack1, int inStep1, int inVelocity)
{
	GetEvent (inTrack1, inStep1).mData2
		= inVelocity > 127 ? 127 : inVelocity;

	int	track0 (inTrack1 - 1);
	int	step0 (inStep1 - 1);

	FireEventChangedAction (track0, step0);
}

void
MRPSequencer::SetScale (int inBars, int inSteps)
{
	if (inSteps < 0 || inSteps > 64)
	{
		return;
	}

	mTicksInSequence = inBars * 4 * kTicksPerQuarterNote;
	mTicksInLoop = mTicksInSequence;
	mTicksPerStep = mTicksInSequence / inSteps;

	printf ("mTicksInSequence = %d\n", mTicksInSequence);
	printf ("mTicksInLoop = %d\n", mTicksInLoop);
	printf ("mTicksPerStep = %d\n", mTicksPerStep);
	printf ("GetStepCount() = %d\n", GetStepCount ());

	FireScaleChangedAction ();
}

void
MRPSequencer::Stop ()
{
	if (mClock->IsStarted ())
	{
		mClock->Stop ();

		FireStopAction ();
	}
}

void
MRPSequencer::TimerTick ()
{
	while (mSequencerIsBusy);

	mSequencerIsBusy = true;

	mTicks++;
	mTotalTicks++;

	if (mTicks >= mTicksInLoop)
	{
		mTicks = 0;
	}

	// work out where we are
	int	ticksBetweenSteps = mTicks % mTicksPerStep;
	int	step = mTicks / mTicksPerStep;

	if (ticksBetweenSteps == 0)
	{
		FireStepTickAction ();
	}
	else
	{
		FireTimerTickAction ();
	}

	for (int t = 0; t < GetTrackCount (); t++)
	{
		MRPTrack	&track (mTracks [t]);
		MRPEvent	&event (track.GetEvent (step + 1));
		MRPEventStatus	&status (track.GetEventStatus (step + 1));

		// is this a note on event
		// and the track is not muted?
		if (ticksBetweenSteps == 0 && (!track.IsMuted ()))
		{
			// is this step vsolo'd?
			if (mVSoloed [step])
			{
				// is this the track which has this step vsolo'd?
				if (status.mVSoloed && !status.mMuted)
				{
					// yep - send the message
					SendMessage (&event, &status);
				}
			}
			// is this track hsolo'd?
			else
			if (track.GetHSoloCount () > 0)
			{
				// is this step on the hsolo bus? 
				if (status.mHSoloed && !status.mMuted)
				{
					// yep - send the message
					SendMessage (&event, &status);
				}
			}
			else
			{
				// no vsolo or hsolo nonsense going on
				if (! status.mMuted)
				{
					// yep - send the message
					SendMessage (&event, &status);
				}
			}
		}
		else
		{
			if (status.mIsNoteOn)
			{
				// work out how many ticks this event's gate time is
				int	gateTicks = GetGateTicks (event.mGateTime);

				if (ticksBetweenSteps >= gateTicks)
				{
					SendNoteOff (&event, &status);
				}
			}
		}
	}

	if (ticksBetweenSteps == 0)
	{
		FireStepTickPostAction ();
	}
	else
	{
		FireTimerTickPostAction ();
	}

	mSequencerIsBusy = false;
}

// step-mute
void
MRPSequencer::UnMute (int inTrack1, int inStep1)
{
	GetTrack (inTrack1).UnMute (inStep1);

	int	step0 (inStep1 - 1);
	int	track0 (inTrack1 - 1);

	FireEventChangedAction (track0, step0);
}

// track-mute
void
MRPSequencer::UnMuteTrack (int inTrack1)
{
	GetTrack (inTrack1).UnMute ();

	int	track0 (inTrack1 - 1);

	FireTrackChangedAction (track0);
}

int
MRPSequencer::UnSelectEvent (int inTrack1, int inStep1)
{
	int	unselections = 0;

	int	step0 (inStep1 - 1);
	int	track0 (inTrack1 - 1);

	if (track0 < 0 || track0 >= GetTrackCount ())
	{
		// bad track
	}
	else
	if (step0 < 0 || step0 >= GetStepCount ())
	{
		// bad step
	}
	else
	{
		GetEventStatus0 (track0, step0).mSelected = false;

		unselections = 1;

		FireEventChangedAction (track0, step0);
	}

	return unselections;
}

int
MRPSequencer::UnSelectStep (int inStep1)
{
	int	unselections = 0;

	int	step0 (inStep1 - 1);

	if (step0 < 0 || step0 >= GetStepCount ())
	{
		// bad step
	}
	else
	{
		for (int t = 0; t < GetTrackCount (); t++, unselections++)
		{
			GetEventStatus0 (t, step0).mSelected = false;

			FireEventChangedAction (t, step0);
		}
	}

	return unselections;
}

int
MRPSequencer::UnSelectTrack (int inTrack1)
{
	int	unselections = 0;

	int	track0 (inTrack1 - 1);

	if (track0 < 0 || track0 >= GetTrackCount ())
	{
		// bad track
	}
	else
	{
		for (int s = 0; s < GetStepCount (); s++, unselections++)
		{
			GetEventStatus0 (track0, s).mSelected = false;

			FireEventChangedAction (track0, s);
		}
	}

	return unselections;
}

// TODO this should be (two?) SelectionActions
int
MRPSequencer::VarySelection ()
{
	int	selectionCount (0);
	vector <MRPEvent>	events;

	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < GetStepCount (); s++)
		{
			const MRPEvent &event (GetEvent0 (t, s));
			const MRPEventStatus &status (GetEventStatus0 (t, s));

			if (status.IsSelected ())
			{
				events.push_back (event);

				selectionCount++;
			}
		}
	}

	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < GetStepCount (); s++)
		{
			const MRPEventStatus &status (GetEventStatus0 (t, s));

			if (status.IsSelected ())
			{
				int	index = rand () % events.size ();

				SetEvent0 (t, s, events [index]);

				// this is crap STL
				vector <MRPEvent>::iterator	event (events.begin ());

				for (int i = 0; i < index; i++)
				{
					// nothing, just bump the iterator
					event++;
				}

				// delete this event
				events.erase (event);
			}
		}
	}

	return selectionCount;
}

// TODO this should be (two?) SelectionActions
int
MRPSequencer::VarySelection2 ()
{
	int	selectionCount (0);
	vector <unsigned char>	notes;

	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < GetStepCount (); s++)
		{
			const MRPEvent &event (GetEvent0 (t, s));
			const MRPEventStatus &status (GetEventStatus0 (t, s));

			if (status.IsSelected ())
			{
				// add the note uniquely to the "palette"
				bool	found (false);

				for (vector <unsigned char>::iterator n = notes.begin ();
					n != notes.end (); n++)
				{
					// uniqueness based on note value
					if (*n == event.mData1)
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					notes.push_back (event.mData1);
				}

				selectionCount++;
			}
		}
	}

	// now set each event in the selection
	// to a random pick from the "palette"
	for (int t = 0; t < GetTrackCount (); t++)
	{
		for (int s = 0; s < GetStepCount (); s++)
		{
			const MRPEventStatus &status (GetEventStatus0 (t, s));

			if (status.IsSelected ())
			{
				int	index = rand () % notes.size ();

				// 1-based
				SetNote (t + 1, s + 1, notes [index]);
			}
		}
	}

	return selectionCount;
}

// vsolo bus is EXCLUSIVE
void
MRPSequencer::VSolo (int inTrack1, int inStep1)
{
	int	step0 (inStep1 - 1);

	// scoot through track
	for (int t = 1; t <= GetTrackCount (); t++)
	{
		// 1-based
		MRPTrack	&track (GetTrack (t));

		// if this is the target track
		if (t == inTrack1)
		{
			track.GetEventStatus (inStep1).mVSoloed = true;
		}
		else
		{
			track.GetEventStatus (inStep1).mVSoloed = false;
		}

		// 1-based
		mVSoloed [step0] = track.GetEventStatus (inStep1).mVSoloed;

		int	track0 (t - 1);

		// 0-based
		FireEventChangedAction (track0, step0);
	}
}

// vsolo bus is EXCLUSIVE
void
MRPSequencer::VUnSolo (int inTrack1, int inStep1)
{
	GetEventStatus (inTrack1, inStep1).mVSoloed = false;

	int	step0 (inStep1 - 1);

	mVSoloed [step0] = false;

	int	track0 (inTrack1 - 1);

	// 0-based
	FireEventChangedAction (track0, step0);
}

// PROTECTED ACTION FIRE METHODS

void
MRPSequencer::FireBPMChangedAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->BPMChanged (this);
	}
}

void
MRPSequencer::FireClockResetAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->ClockReset (this);
	}
}

void
MRPSequencer::FireConfigChangedAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->ConfigChanged (this);
	}
}

void
MRPSequencer::FireEventChangedAction (int inTrack0, int inStep0)
{
	int	track1 (inTrack0 + 1);
	int	step1 (inStep0 + 1);

	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->EventChanged (this, track1, step1);
	}
}

void
MRPSequencer::FireLoopChangedAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->LoopChanged (this);
	}
}

void
MRPSequencer::FirePauseAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->Pause (this);
	}
}

void
MRPSequencer::FirePortListChangedAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->PortListChanged (this);
	}
}

void
MRPSequencer::FireScaleChangedAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->ScaleChanged (this);
	}
}

void
MRPSequencer::FireStartAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->Start (this);
	}
}

void
MRPSequencer::FireStepTickAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->StepTick (this);
	}
}

void
MRPSequencer::FireStepTickPostAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->StepTickPost (this);
	}
}

void
MRPSequencer::FireStopAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->Stop (this);
	}
}

void
MRPSequencer::FireTimerTickAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->TimerTick (this);
	}
}

void
MRPSequencer::FireTimerTickPostAction ()
{
	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->TimerTickPost (this);
	}
}

void
MRPSequencer::FireTrackChangedAction (int inTrack0)
{
	int	track1 (inTrack0 + 1);

	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->TrackChanged (this, track1);
	}
}

void
MRPSequencer::FireTrackAddedAction (int inTrack0)
{
	int	track1 (inTrack0 + 1);

	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->TrackAdded (this, track1);
	}
}

void
MRPSequencer::FireTrackDeletedAction (int inTrack0)
{
	int	track1 (inTrack0 + 1);

	for (int l = 0; l < (int) mListeners.size (); l++)
	{
		mListeners [l]->TrackDeleted (this, track1);
	}
}

// PROTECTED METHODS

MRPEvent &
MRPSequencer::GetEvent0 (int inTrack0, int inStep0)
{
	int	step1 (inStep0 + 1);
	int	track1 (inTrack0 + 1);

	return GetEvent (track1, step1);
}

const MRPEvent &
MRPSequencer::GetEvent0 (int inTrack0, int inStep0) const
{
	int	step1 (inStep0 + 1);
	int	track1 (inTrack0 + 1);

	return GetEvent (track1, step1);
}

MRPEventStatus &
MRPSequencer::GetEventStatus0 (int inTrack0, int inStep0)
{
	int	step1 (inStep0 + 1);
	int	track1 (inTrack0 + 1);

	return GetEventStatus (track1, step1);
}

const MRPEventStatus &
MRPSequencer::GetEventStatus0 (int inTrack0, int inStep0) const
{
	int	step1 (inStep0 + 1);
	int	track1 (inTrack0 + 1);

	return GetEventStatus (track1, step1);
}

int
MRPSequencer::GetGateTicks (int inGatePercent)
{
	double	ticks = (inGatePercent / 100.0) * mTicksPerStep;

	return (int) ticks;
}

void
MRPSequencer::SetEvent0 (int inTrack0, int inStep0, const MRPEvent &inEvent)
{
	int	track1 (inTrack0 + 1);
	int	step1 (inStep0 + 1);

	SetEvent (track1, step1, inEvent);

	FireEventChangedAction (inTrack0, inStep0);
}

