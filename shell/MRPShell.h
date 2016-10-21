// MRPShell.h

#ifndef MRPShell_h
#define MRPShell_h

// INCLUDES

#include <string>
#include <vector>

#include <AudioToolbox/MusicPlayer.h>

#include "MRPSequencerListener.h"

// NAMESPACE

using namespace std;

// FORWARD DECLARATIONS

class MRPSequencer;
class MRPShell;
class MRPShellInterface;

// MRPSHELL

class MRPShell
	:
	public MRPSequencerListener
{
	// public constructors
	public:

		MRPShell (MRPSequencer *inSequencer, MRPShellInterface *inInterface);

                ~MRPShell();

	// MRPSequencerListener implementation
	public:

		void
		StepTickPost (MRPSequencer *inSequencer);

	// public methods
	public:

		int
		Execute (const char *inBuffer);

		string
		GetHistoryEntryByNumber (int inIndex);

		string
		GetHistoryEntryBySubString (const char *inReference);

		void
		SetConfirmSaves (bool inConfirm);

		void
		Shell (bool inPrompt);

	// protected methods
	protected:

		int
		Dispatch ();

		pair <int, string>
		GetHistoryEntryByIndex (int inIndex);

		int
		GetHistoryCount ();

		// trackreference is either a number or a track name
		int
		GetTrackNumber (string inTrackReference);

		void
		Parse (const char *inBuffer);

		string
		ParseOne (const char *inBuffer);

		string
		RebuildCommandLine (const vector <string> &inCommands);

		bool
		SatisfyHistoryReferences (vector <string> *ioCommands);

		void
		SaveToHistory (string inCommand);

		int
		Split (const char *inBuffer, vector <string> *outCommands);

		void
		StripSpaces (string *ioString);

	// equates
	private:

		typedef enum
		{
			kMaxHistory = 50
		} MaxHistoryType;

	// static command handlers
	private:

		static int ChangeDirectory (MRPShell *inShell);
		static int ClearSelection (MRPShell *inShell);
		static int CopyTrack (MRPShell *inShell);
		static int BPM (MRPShell *inShell);
		static int ChangeChannel (MRPShell *inShell);
		static int ChangeNote (MRPShell *inShell);
		static int DefaultSequence (MRPShell *inShell);
		static int DeleteConfig (MRPShell *inShell);
		static int DeleteSequence (MRPShell *inShell);
		static int DeleteTrack (MRPShell *inShell);
		static int DirectoryListing (MRPShell *inShell);
		static int Display (MRPShell *inShell);
		static int ExportSMF (MRPShell *inShell);
		static int FitToKey (MRPShell *inShell);
		static int Help (MRPShell *inShell);
		static int History (MRPShell *inShell);
		static int HSolo (MRPShell *inShell);
		static int HUnSolo (MRPShell *inShell);
		static int InternalSync (MRPShell *inShell);
		static int InvertMute (MRPShell *inShell);
		static int InvertSelection (MRPShell *inShell);
		static int ImportSMF (MRPShell *inShell);
		static int Load (MRPShell *inShell);
		static int MIDISync (MRPShell *inShell);
		static int Migration (MRPShell *inShell);
		static int Mute (MRPShell *inShell);
		static int MuteTrack (MRPShell *inShell);
		static int NameChannel (MRPShell *inShell);
		static int NameTrack (MRPShell *inShell);
		static int NewTrack (MRPShell *inShell);
		static int Pause (MRPShell *inShell);
		static int Play (MRPShell *inShell);
		static int Ports (MRPShell *inShell);
		static int Quit (MRPShell *inShell);
		static int Read (MRPShell *inShell);
		static int Reset (MRPShell *inShell);
		static int RandomiseChannel (MRPShell *inShell);
		static int RandomiseData1 (MRPShell *inShell);
		static int RandomiseData2 (MRPShell *inShell);
		static int RandomiseGate (MRPShell *inShell);
		static int RandomiseMute (MRPShell *inShell);
		static int RandomiseNote (MRPShell *inShell);
		static int RandomisePort (MRPShell *inShell);
		static int RandomiseVelocity (MRPShell *inShell);
		static int Rotate (MRPShell *inShell);
		static int Save (MRPShell *inShell);
		static int SaveConfig (MRPShell *inShell);
		static int SaveSelection (MRPShell *inShell);
		static int SelectAll (MRPShell *inShell);
		static int SelectEighthNotes (MRPShell *inShell);
		static int SelectEvent (MRPShell *inShell);
		static int SelectQuarterNotes (MRPShell *inShell);
		static int SelectStep (MRPShell *inShell);
		static int SelectTrack (MRPShell *inShell);
		static int SetChannel (MRPShell *inShell);
		static int SetData1 (MRPShell *inShell);
		static int SetData2 (MRPShell *inShell);
		static int SetGateTime (MRPShell *inShell);
		static int SetLoop (MRPShell *inShell);
		static int SetNote (MRPShell *inShell);
		static int SetPort (MRPShell *inShell);
		static int SetStepAction (MRPShell *inShell);
		static int SetScale (MRPShell *inShell);
		static int SetType (MRPShell *inShell);
		static int SetVelocity (MRPShell *inShell);
		static int Shell (MRPShell *inShell);
		static int Stop (MRPShell *inShell);
		static int UnMute (MRPShell *inShell);
		static int UnMuteTrack (MRPShell *inShell);
		static int UnSelectEvent (MRPShell *inShell);
		static int UnSelectStep (MRPShell *inShell);
		static int UnSelectTrack (MRPShell *inShell);
		static int Vary (MRPShell *inShell);
		static int Vary2 (MRPShell *inShell);
		static int Version (MRPShell *inShell);
		static int VSolo (MRPShell *inShell);
		static int VUnSolo (MRPShell *inShell);

	// private type definitions
	private:

		typedef struct
		{
			char	*mCommand;
			char	*mAlias;
			char	*mDescription;
			int (*mHandler) (MRPShell *inShell);

		} Command;
	
	// static private data members
	private:

		static Command
		sCommands [];

		static const char *
		sNoteNames [];

	// private data members
	private:

		MRPSequencer *
		mSequencer;

		MRPShellInterface *
		mInterface;

		bool
		mConfirmSaves;

		bool
		mMigrationMode;

		int
		mHistoryNumber;

		vector <pair <int, string> >
		mHistory;

		vector <string>
		mParameters;

		vector <string>
		mStepActions;
};

#endif	// MRPShell_h


