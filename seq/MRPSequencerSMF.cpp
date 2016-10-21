// MRPSequencerSMF.cpp

// HEADERS

#include "MRPSequencerSMF.h"

#include "MRPSequencer.h"
#include "MRPShellInterface.h"

// PUBLIC CONSTRUCTOR

MRPSequencerSMF::MRPSequencerSMF (MRPSequencer *inSequencer, MRPShellInterface *inInterface)
	:
	mSequencer (inSequencer),
	mInterface (inInterface)
{
}

// PUBLIC METHODS

int
MRPSequencerSMF::Export (const char *inFileName)
{
	// this is the step length
	Float64	stepLength = (double) mSequencer->GetTicksPerStep ()
		/ (double) mSequencer->GetTicksPerQuarterNote ();

	// this is cumulative time stamp
	Float64	timeStamp = 0.0;

	MIDINoteMessage	message;
	MusicSequence	sequence;
	MusicTrack	track;

	OSStatus	status = NewMusicSequence (&sequence);

	if (status == 0)
	{
		// get the tempo track
		status = MusicSequenceGetTempoTrack (sequence, &track);

		ClearTrack (track);

		status = MusicTrackNewExtendedTempoEvent
			(track, 0.0, mSequencer->GetBPM ());

		mInterface->WriteFormattedLD ("MusicTrackNewExtendedTempoEvent returns %ld\n",
			status);

		for (int t = 1; t <= mSequencer->GetTrackCount (); t++)
		{
			status = MusicSequenceNewTrack (sequence, &track);

			if (status == 0)
			{
				for (int s = 1; s <= mSequencer->GetStepCount (); s++)
				{
					const MRPEvent	&event (mSequencer->GetEvent (t, s));

					message.channel = event.mChannel;
					message.note = event.mData1;
					message.velocity = event.mData2;

					// duration is gate percent * step length
					message.duration = (Float64) event.mGateTime / 100.0;
					message.duration *= stepLength;

					status = MusicTrackNewMIDINoteEvent
						(track, timeStamp, &message);

					timeStamp += stepLength;
				}
			}
			else
			{
				mInterface->WriteFormattedLD ("MusicSequenceNewTrack returns %ld\n", status);
			}
		}

		UInt32	trackCount (0);

		if (MusicSequenceGetTrackCount (sequence, &trackCount) == 0
			&& trackCount > 0)
		{
			Boolean	isDirectory;
			FSRef	ref;
			FSSpec	fspec = {0, 0, ""};

			OSErr	errCode = FSPathMakeRef
				((const unsigned char *) inFileName, &ref, &isDirectory);

			mInterface->WriteFormattedSD ("FSPathMakeRef (%s) returns %d\n", 
				inFileName, errCode);

			if (errCode == fnfErr)
			{
				// shit, have to take the long way round
				// because, for some retarded reason
				// you can't have an FSRef to a nonexistent path
				// the way you can with an FSSpec

				// start by finding a path to the parent directory
				char	parentDirectory [128];

				char	*leaf = strrchr (inFileName, '/');
				
				int	lastSlash = -1;

				if (leaf != NULL)
				{
					lastSlash = strlen (inFileName) - strlen (leaf);
				}

				if (lastSlash == -1)
				{
					strcpy (parentDirectory, ".");
				}
				else
				{
					strncpy (parentDirectory, inFileName, lastSlash);
					parentDirectory [lastSlash] = 0;
				}

				errCode = FSPathMakeRef
					((const unsigned char *) parentDirectory, &ref, &isDirectory);

				if (errCode == noErr)
				{
					FSCatalogInfo	catInfo;

					FSGetCatalogInfo (&ref, kFSCatInfoNodeID | kFSCatInfoVolume,
						&catInfo, NULL, &fspec, NULL);

					mInterface->WriteFormattedD ("FSGetCatalogInfo returns %d\n", errCode);

					Str255	leafName;

					if (leaf == NULL)
					{
						strcpy ((char *) &leafName [1], inFileName);
						leafName [0] = strlen (inFileName);
					}
					else
					{
						int	leafLength = strlen (leaf) - 1;

						strncpy ((char *) &leafName [1], leaf + 1, leafLength);
						leafName [leafLength + 1] = 0;
						leafName [0] = leafLength;
					}

					FSMakeFSSpec
						(catInfo.volume, catInfo.nodeID, leafName, &fspec);

					// terminate the results of the MakeFSSpec call
					fspec.name [fspec.name [0] + 1] = 0;
				}
				else
				{
					// all right, fuck off Apple
					mInterface->WriteFormattedD ("FSPathMakeRef returns %d\n", errCode);
				}
			}
			else
			{
				FSGetCatalogInfo
					(&ref, kFSCatInfoVolume, NULL, NULL, &fspec, NULL);

				mInterface->WriteFormattedLD ("FSGetCatalogInfo returns %d\n", errCode);
			}

			status = MusicSequenceSaveSMF (sequence, &fspec, 0);

			mInterface->WriteFormattedLD ("MusicSequenceSaveSMF returns %ld\n", status);
		}

		DisposeMusicSequence (sequence);
	}

	return 0;
}

int
MRPSequencerSMF::Import (const char *inFileName)
{
	MusicSequence	sequence;

	OSStatus	status = NewMusicSequence (&sequence);

	if (status == 0)
	{
		Boolean	isDirectory (false);
		FSRef	ref;

		status = FSPathMakeRef
			((const unsigned char *) inFileName, &ref, &isDirectory);

		if (status == 0)
		{
			status = MusicSequenceLoadSMFWithFlags (sequence, &ref, 0);

			if (status == 0)
			{
				UInt32	trackCount (0);

				double	bpm (GetTempo (sequence));

				if (mSequencer->GetTrackCount () == 0)
				{
					mSequencer->SetBPM (bpm);
				}

				status = MusicSequenceGetTrackCount (sequence, &trackCount);

				mInterface->WriteFormattedLU ("MRPShell: %lu tracks in sequence\n", trackCount);

				for (UInt32 i = 0; i < trackCount; i++)
				{
					MusicTrack	track;

					OSStatus	status = MusicSequenceGetIndTrack
						(sequence, i, &track);

					if (status == 0)
					{
						int	trackNumber = LoadTrack (track);

						if (trackNumber != -1)
						{
							mInterface->WriteFormattedD ("loaded track %d\n", trackNumber);
						}
					}
				}
			}
			else
			{
				mInterface->WriteFormattedLD ("MusicSequenceLoadSMF returns %ld\n", status);
			}
		}
		else
		{
			mInterface->WriteFormattedLD ("FSPathMakeRef returns %ld\n", status);
		}

		DisposeMusicSequence (sequence);
	}
	else
	{
		mInterface->WriteFormattedLD ("NewMusicSequence returns %ld\n", status);
	}

	return 0;
}

// PRIVATE METHODS

void
MRPSequencerSMF::ClearTrack (MusicTrack inTrack)
{
	Boolean	hasCurrentEvent (false);
	MusicEventIterator	iterator;

	OSStatus	status = NewMusicEventIterator (inTrack, &iterator);

	if (status == 0)
	{
		MusicEventIteratorHasCurrentEvent
			(iterator, &hasCurrentEvent);

		while (hasCurrentEvent)
		{
			status = MusicEventIteratorDeleteEvent (iterator);

			if (status == 0)
			{
				// NEXT

				status = MusicEventIteratorNextEvent (iterator);

				if (status == 0)
				{
					MusicEventIteratorHasCurrentEvent
						(iterator, &hasCurrentEvent);
				}
				else
				{
					hasCurrentEvent = false;
				}
			}
		}
	}
	
}

double
MRPSequencerSMF::GetTempo (MusicSequence inSequence)
{
	const void	*eventData (NULL);
	Boolean	hasCurrentEvent (false);
	UInt32	eventDataSize;
	double	bpm (0.0);
	MusicEventIterator	iterator;
	MusicEventType	eventType;
	MusicTimeStamp	eventTimeStamp;
	MusicTrack	tempoTrack;

	OSStatus	status = MusicSequenceGetTempoTrack
		(inSequence, &tempoTrack);

	if (status == 0)
	{
		status = NewMusicEventIterator (tempoTrack, &iterator);

		if (status == 0)
		{
			MusicEventIteratorHasCurrentEvent
				(iterator, &hasCurrentEvent);

			while (hasCurrentEvent)
			{
				status = MusicEventIteratorGetEventInfo
					(iterator, &eventTimeStamp, &eventType,
						&eventData, &eventDataSize);

				if (status == 0)
				{
					// WORK

					if (eventType == kMusicEventType_ExtendedTempo)
					{
						ExtendedTempoEvent	*tempoEvent
							((ExtendedTempoEvent *) eventData);

						bpm = tempoEvent->bpm;

						mInterface->WriteFormattedLF ("MRPShell: found tempo event for %lf bpm\n", bpm);

						break;
					}

					// NEXT

					status = MusicEventIteratorNextEvent (iterator);

					if (status == 0)
					{
						MusicEventIteratorHasCurrentEvent
							(iterator, &hasCurrentEvent);
					}
					else
					{
						hasCurrentEvent = false;
					}
				}
			}
		}
	}
	else
	{
		mInterface->Write ("could not get tempo track\n");
	}

	return bpm;
}

int
MRPSequencerSMF::LoadTrack (MusicTrack inTrack)
{
	const void	*eventData (NULL);
	MIDINoteMessage	*noteMessage (NULL);
	MRPTrack	*newTrack (NULL);
	Boolean	hasCurrentEvent (false);
	int	eventNumber (1);
	UInt32	eventDataSize;
	MusicEventIterator	iterator;
	MusicEventType	eventType;
	MusicTimeStamp	eventTimeStamp;

	OSStatus	status = NewMusicEventIterator (inTrack, &iterator);

	MusicEventIteratorHasCurrentEvent
		(iterator, &hasCurrentEvent);

	while (hasCurrentEvent)
	{
		status = MusicEventIteratorGetEventInfo
			(iterator, &eventTimeStamp, &eventType,
				&eventData, &eventDataSize);

		if (status == 0)
		{
			// WORK

			if (eventType == kMusicEventType_MIDINoteMessage)
			{
				if (newTrack == NULL)
				{
					newTrack = new MRPTrack ();
				}

				if (eventNumber <= mSequencer->GetStepCount ())
				{
					noteMessage = (MIDINoteMessage *) eventData;

					MRPEvent	&event (newTrack->GetEvent (eventNumber));

					// default gate time to 50%
					event.mChannel = (noteMessage->channel - 1);
					event.mData1 = noteMessage->note;
					event.mPort = 0;
					event.mData2 = noteMessage->velocity;
					event.mGateTime = 50;

					eventNumber++;
				}
			}

			// NEXT

			status = MusicEventIteratorNextEvent (iterator);

			if (status == 0)
			{
				MusicEventIteratorHasCurrentEvent
					(iterator, &hasCurrentEvent);
			}
			else
			{
				hasCurrentEvent = false;
			}
		}

	}

	if (newTrack != NULL)
	{
		// fill any remaining event slots
		// with the last event detected
		for (; eventNumber < mSequencer->GetStepCount (); eventNumber++)
		{
			MRPEvent	&event (newTrack->GetEvent (eventNumber));

			// default gate time to 50%
			event.mChannel = (noteMessage->channel - 1);
			event.mData1 = noteMessage->note;
			event.mPort = 0;
			event.mData2 = noteMessage->velocity;
			event.mGateTime = 50;
		}
	}

	int	trackNumber = -1;

	if (newTrack != NULL)
	{
		trackNumber = mSequencer->AddTrack (*newTrack);

		delete newTrack;
		newTrack = NULL;
	}

	return trackNumber;
}


