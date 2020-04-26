// MRPShell.cpp

// INCLUDES

#include "MRPShell.h"

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <sys/param.h>

#include <string>

#include <AudioToolbox/MusicPlayer.h>

#include "MIDICLClient.h"
#include "MIDICLInputPort.h"
#include "MIDICLOutputPort.h"

#include "MRPEvent.h"
#include "MRPSelectionAction.h"
#include "MRPSelectionFitToKey.h"
#include "MRPSequencer.h"
// #include "MRPSequencerSMF.h"
#include "MRPStdioInterface.h"
#include "MRPTrack.h"

// DEFINES

// STATIC INITIALISATION

MRPShell::Command MRPShell::sCommands [] =
{
	{"cd", "", "change directory", MRPShell::ChangeDirectory},
	{"clearselection", "clear", "clear selection", MRPShell::ClearSelection},
	{"copytrack", "copyt", "copy track (from to)", MRPShell::CopyTrack},
	{"bpm", "", "set bpm (bpm)", MRPShell::BPM},
	{"changechannel", "", "changechannel (from to)", MRPShell::ChangeChannel},
	{"changenote", "", "changenote (from to)", MRPShell::ChangeNote},
	{"default", "", "default sequence", MRPShell::DefaultSequence},
	{"deleteconfig", "", "delete config", MRPShell::DeleteConfig},
	{"deletesequence", "", "delete sequence", MRPShell::DeleteSequence},
	{"deletetrack", "delt", "delete track", MRPShell::DeleteTrack},
	{"ls", "dir", "directory listing", MRPShell::DirectoryListing},
	{"display", "di", "display sequence", MRPShell::Display},
//	{"exportsmf", "", "export (smf file)", MRPShell::ExportSMF},
	{"fitkey", "", "fit to key (key)", MRPShell::FitToKey},
	{"help", "", "display this message", MRPShell::Help},
	{"history", "", "display command history", MRPShell::History},
	{"hsolo", "hs", "horizontal solo", MRPShell::HSolo},
	{"hunsolo", "hu", "horizontal unsolo", MRPShell::HUnSolo},
//	{"importsmf", "", "import (smf file)", MRPShell::ImportSMF},
	{"internalsync", "", "internal sync", MRPShell::InternalSync},
	{"invertmute", "im", "invert mute", MRPShell::InvertMute},
	{"invertselection", "is", "invert selection", MRPShell::InvertSelection},
	{"load", "", "delete sequence and read (file)", MRPShell::Load},
	{"midisync", "", "midi sync (port)", MRPShell::MIDISync},
	{"migration", "", "load 0-based scripts", MRPShell::Migration},
	{"mute", "mu", "mute", MRPShell::Mute},
	{"mutetrack", "mt", "mute track (track)", MRPShell::MuteTrack},
	{"namechannel", "", "name channel [port] (channel)", MRPShell::NameChannel},
	{"nametrack", "", "name track (track name)", MRPShell::NameTrack},
	{"newtrack", "newt", "new track [track]", MRPShell::NewTrack},
	{"pause", "", "pause", MRPShell::Pause},
	{"play", "", "play", MRPShell::Play},
	{"ports", "", "list output ports", MRPShell::Ports},
	{"quit", "", "stop & quit", MRPShell::Quit},
	{"read", "", "read script (file)", MRPShell::Read},
	{"reset", "", "reset clock", MRPShell::Reset},
	{"randomchannel", "", "randomise channel [base range]", RandomiseChannel},
	{"randomdata1", "", "randomise data1 [base range]", RandomiseData1},
	{"randomdata2", "", "randomise data2 [base range]", RandomiseData2},
	{"randomgate", "", "randomise gate [base range]", RandomiseGate},
	{"randommute", "", "randomise mute", RandomiseMute},
	{"randomnote", "", "randomise note [base range]", RandomiseNote},
	{"randomport", "", "randomise port [base range]", RandomisePort},
	{"randomvelocity", "", "randomise velocity [base range]", RandomiseVelocity},
	{"rotate", "", "rotate [count]", MRPShell::Rotate},
	{"save", "", "save (file)", MRPShell::Save},
	{"saveconfig", "", "save configuration (file)", MRPShell::SaveConfig},
	{"saveselection", "savesel", "save selection script (file)", MRPShell::SaveSelection},
	{"selectall", "", "select all", MRPShell::SelectAll},
	{"selecteighthnotes", "sel8", "select eighth notes within selection", MRPShell::SelectEighthNotes},
	{"selectevent", "sele", "select event (track step)", SelectEvent},
	{"selectquarternotes", "sel4", "select quarter notes within selection", MRPShell::SelectQuarterNotes},
	{"selectstep", "sels", "select step (step)", MRPShell::SelectStep},
	{"selecttrack", "selt", "select track (track)", MRPShell::SelectTrack},
	{"setchannel", "", "set channel (channel)", MRPShell::SetChannel},
	{"setgatetime", "setgate", "set gate (percent)", MRPShell::SetGateTime},
	{"setdata1", "", "set data1 (data) // note,bendlsb,control,etc", MRPShell::SetData1},
	{"setdata2", "", "set data2 (data) // velocity,bendmsb,value,etc", MRPShell::SetData2},
	{"setloop", "", "set loop (step)", MRPShell::SetLoop},
	{"setnote", "", "set note (note)", MRPShell::SetNote},
	{"setport", "", "set port (port)", MRPShell::SetPort},
	{"setscale", "", "set scale (bars, steps)", MRPShell::SetScale},
	{"setstepaction", "", "set step action (step, action(s))", MRPShell::SetStepAction},
	{"settype", "", "set event type (type - note,program,pressure,control,polypressure)", MRPShell::SetType},
	{"setvelocity", "", "set velocity (velocity)", MRPShell::SetVelocity},
	{"shell", "sh", "run shell command (command params)", MRPShell::Shell},
	{"stop", "", "pause & reset", MRPShell::Stop},
	{"unmute", "unmu", "unmute", MRPShell::UnMute},
	{"unmutetrack", "unmt", "unmute track (track)", MRPShell::UnMuteTrack},
	{"unselectevent", "unsele", "unselect event (track step)", UnSelectEvent},
	{"unselectstep", "unsels", "unselect step (step)", MRPShell::UnSelectStep},
	{"unselecttrack", "unselt", "unselect track (track)", MRPShell::UnSelectTrack},
	{"vary", "", "vary (algorithm 1)", MRPShell::Vary},
	{"vary2", "", "vary (algorithm 2)", MRPShell::Vary2},
	{"version", "ver", "print version", MRPShell::Version},
	{"vsolo", "vs", "vertical solo", MRPShell::VSolo},
	{"vunsolo", "vu", "vertical unsolo", MRPShell::VUnSolo}
};

#define kNumCommands \
	sizeof (sCommands) / sizeof (sCommands [0])

// PUBLIC CONSTRUCTOR

MRPShell::MRPShell (MRPSequencer *inSequencer, MRPShellInterface *inInterface)
	:
	mSequencer (inSequencer),
	mInterface (inInterface),
	mConfirmSaves (true),
	mMigrationMode (false),
	mHistoryNumber (1)
{
	mSequencer->AddListener (this);
}


MRPShell::~MRPShell()
{
	mSequencer->RemoveListener(this);
}



// MRPSEQUENCERLISTENER IMPLEMENTATION

void
MRPShell::StepTickPost (MRPSequencer *inSequencer)
{
	int	step0 (inSequencer->GetCurrentStep ());

	if (step0 < (int) mStepActions.size ())
	{
		string	stepAction (mStepActions [step0]);

		if (stepAction.length () > 0)
		{
			Execute (stepAction.c_str ());
		}
	}
}

// PUBLIC METHODS

int
MRPShell::Execute (const char *inBuffer)
{
	int	result = 0;

	int	length = strlen (inBuffer);

	if (length > 0)
	{
		vector <string>	commands;

		if (Split (inBuffer, &commands) > 0)
		{
			if (SatisfyHistoryReferences (&commands))
			{
				string	rebuiltCommandLine (RebuildCommandLine (commands));

				// trash the list
				commands.erase (commands.begin (), commands.end ());

				// history references may include multiple commands
				// so re-split the command line
				if (Split (rebuiltCommandLine.c_str (), &commands) > 0)
				{
					// *now* save off the command into history
					rebuiltCommandLine = RebuildCommandLine (commands);

					// and save it off to the history
					SaveToHistory (rebuiltCommandLine);

					for (unsigned int i = 0; i < commands.size () && result == 0; i++)
					{
						Parse (commands [i].c_str ());

						int	commandNumber = Dispatch ();

						if (commandNumber == -1)
						{
							mInterface->WriteFormattedS
								("%s: command not found\n", mParameters [0].c_str ());
						}
						else
						{
							result = sCommands [commandNumber].mHandler (this);
						}
					}
				}
			}
		}
	}

	return result;
}

string
MRPShell::GetHistoryEntryByNumber (int inIndex)
{
	string	entry;

	if (inIndex < 0)
	{
		inIndex += mHistoryNumber;
	}

	// HACK FIXME
	vector <pair <int, string> >::iterator	i (mHistory.begin ());

	for (; i != mHistory.end (); i++)
	{
		if (i->first == inIndex)
		{
			entry = i->second;
		}
	}

	return entry;
}

string
MRPShell::GetHistoryEntryBySubString (const char *inSubString)
{
	string	entry;

	int	length = strlen (inSubString);

	vector <pair <int, string> >::iterator	i (mHistory.begin ());

	for (; i != mHistory.end (); i++)
	{
		if (strncmp (inSubString, i->second.c_str (), length) == 0)
		{
			entry = i->second;
		}
	}

	return entry;
}

void
MRPShell::SetConfirmSaves (bool inConfirm)
{
	mConfirmSaves = inConfirm;
}

void
MRPShell::Shell (bool inPrompt)
{
	char	buffer [100];
	int	result = 0;

	do
	{
		if (inPrompt)
		{
			mInterface->WriteFormattedD ("%d mrp> ", mHistoryNumber);
		}

		int	length = mInterface->ReadLine (buffer, sizeof (buffer));

		if (length == -1)
		{
			break;
		}

		result = Execute (buffer);
	}
	while (result == 0);
}

// PROTECTED METHODS

int
MRPShell::Dispatch ()
{
	const char	*command = mParameters [0].c_str ();
	int	commandNumber = -1;
	
	for (unsigned int i = 0; i < kNumCommands; i++)
	{
		if (strcmp (command, sCommands [i].mCommand) == 0)
		{
			commandNumber = (int) i;
			break;
		}
		else
		if (sCommands [i].mAlias [0] != 0)
		{
			if (strcmp (command, sCommands [i].mAlias) == 0)
			{
				commandNumber = (int) i;
				break;
			}
		}
	}

	return commandNumber;
}

int
MRPShell::GetHistoryCount ()
{
	return mHistory.size ();
}

pair <int, string>
MRPShell::GetHistoryEntryByIndex (int inIndex)
{
	pair <int, string>	entry;

	if (inIndex < (int) mHistory.size ())
	{
		entry = mHistory [inIndex];
	}

	return entry;
}

int
MRPShell::GetTrackNumber (string inTrackReference)
{
	int	trackNumber = -1;

	if (isdigit (inTrackReference [0]))
	{
		trackNumber = atoi (inTrackReference.c_str ());
	}
	else
	{
		trackNumber = mSequencer->GetTrackNumber (inTrackReference);
	}

	return trackNumber;
}

void
MRPShell::Parse (const char *inBuffer)
{
	bool	escaped (false);
	bool	quoted (false);
	bool	wasSpace (true);
	char	quoteCharacter;
	unsigned int	paramNumber = 0;
	int	bufferLength = strlen (inBuffer);

	mParameters.erase (mParameters.begin (), mParameters.end ());

	for (int i = 0; i < bufferLength; i++)
	{
		char	ch = inBuffer [i];

		if (escaped)
		{
			if (paramNumber >= mParameters.size ())
			{
				mParameters.push_back ("");
			}

			mParameters [paramNumber] += ch;
			escaped = false;
		}
		else if (quoted)
		{
			if (ch == quoteCharacter)
			{
				quoted = false;
			}
			else
			{
				if (paramNumber >= mParameters.size ())
				{
					mParameters.push_back ("");
				}

				mParameters [paramNumber] += ch;
			}
		}
		else
		{
			if (ch == ' ')
			{
				if (! wasSpace)
				{
					wasSpace = true;
					paramNumber++;
				}
			}
			else if (ch == '\\')
			{
				escaped = true;
				wasSpace = false;
			}
			else if (ch == '\'' || ch == '"')
			{
				quoteCharacter = ch;
				quoted = true;
				wasSpace = false;
			}
			else
			{
				wasSpace = false;

				if (paramNumber >= mParameters.size ())
				{
					mParameters.push_back ("");
				}

				mParameters [paramNumber] += ch;
			}
		}
	}
}

string
MRPShell::ParseOne (const char *inBuffer)
{
	bool	escaped (false);
	bool	quoted (false);
	char	quoteCharacter;
	int	bufferLength = strlen (inBuffer);
	string	command;

	// skip leading spaces
	int	i (0);

	for (i = 0; i < bufferLength; i++)
	{
		if (inBuffer [i] != ' ' && inBuffer [i] != '\t')
		{
			break;
		}
	}

	for (; i < bufferLength; i++)
	{
		char	ch = inBuffer [i];

		if (escaped)
		{
			escaped = false;

			command += ch;
		}
		else if (quoted)
		{
			if (ch == quoteCharacter)
			{
				quoted = false;
			}
			else
			{
				command += ch;
			}
		}
		else
		{
			if (ch == '\\')
			{
				escaped = true;
				command += ch;
			}
			else if (ch == '\'' || ch == '"')
			{
				quoteCharacter = ch;
				quoted = true;
				command += ch;
			}
			else if (ch == ' ' || ch == '\t')
			{
				break;
			}
			else
			{
				command += ch;
			}
		}
	}

	return command;
}

string
MRPShell::RebuildCommandLine (const vector <string> &inCommands)
{
	string	rebuiltCommandLine;

	// rebuild the command with history references satisfied
	// note we do not have to worry about quoted arguments
	// as Split() copies quotes
	for (unsigned int i = 0; i < inCommands.size (); i++)
	{
		if (i != 0)
		{
			rebuiltCommandLine += ';';
			rebuiltCommandLine += ' ';
		}

		rebuiltCommandLine += inCommands [i];
	}

	return rebuiltCommandLine;
}

void
MRPShell::SaveToHistory (string inCommand)
{
	pair <int, string>	entry (mHistoryNumber, inCommand);

	mHistoryNumber++;

	mHistory.push_back (entry);

	if (mHistory.size () > kMaxHistory)
	{
		mHistory.erase (mHistory.begin ());
	}
}

int
MRPShell::Split (const char *inBuffer, vector <string> *outCommands)
{
	bool	escaped (false);
	bool	quoted (false);
	char	quoteCharacter;
	int	bufferLength = strlen (inBuffer);
	string	command;

	// skip leading spaces
	int	i (0);

	for (i = 0; i < bufferLength; i++)
	{
		if (inBuffer [i] != ' ' && inBuffer [i] != '\t')
		{
			break;
		}
	}

	for (; i < bufferLength; i++)
	{
		char	ch = inBuffer [i];

		if (escaped)
		{
			escaped = false;

			command += ch;
		}
		else if (quoted)
		{
			if (ch == quoteCharacter)
			{
				quoted = false;
			}

			command += ch;
		}
		else
		{
			if (ch == '\\')
			{
				escaped = true;
				command += ch;
			}
			else if (ch == '\'' || ch == '"')
			{
				quoteCharacter = ch;
				quoted = true;
				command += ch;
			}
			else if (ch == ';')
			{
				if (command.length () > 0)
				{
					StripSpaces (&command);

					outCommands->push_back (command);

					command.erase ();
				}
			}
			else
			{
				command += ch;
			}
		}
	}

	int	numCommands (0);

	if (quoted)
	{
		mInterface->Write ("mismatched quotes\n");
	}
	else
	{
		if (command.length () > 0)
		{
			StripSpaces (&command);

			outCommands->push_back (command);
		}

		numCommands = outCommands->size ();
	}

	return numCommands;
}

void
MRPShell::StripSpaces (string *ioString)
{
	while (ioString->size () > 0 && ioString->operator [] (0) == ' ')
	{
		ioString->erase (0, 1);
	}

	while (ioString->size () > 0 && ioString->operator [] (ioString->size () - 1) == ' ')
	{
		ioString->erase (ioString->size () - 1, 1);
	}
}

bool
MRPShell::SatisfyHistoryReferences (vector <string> *ioCommands)
{
	bool	success (true);
	int	expansions (0);

	for (unsigned int i = 0; i < ioCommands->size (); i++)
	{
		string	command (ParseOne (ioCommands->operator [] (i).c_str ()));

		if (command [0] == '!' && command.length () > 1)
		{
			const char	*historyReference
				(ioCommands->operator [] (i).c_str () + 1);

			if (isdigit (historyReference [0]) || historyReference [0] == '-')
			{
				int	commandNumber = atoi (historyReference);
				command = GetHistoryEntryByNumber (commandNumber);
			}
			else
			{
				command = GetHistoryEntryBySubString (historyReference);
			}

			if (command.length () == 0)
			{
				if (historyReference [0] == '!')
				{
					command = GetHistoryEntryByNumber (-1);

					if (command.length () == 0)
					{
						mInterface->WriteFormattedS
							("%s: event not found\n", historyReference);

						success = false;

						break;
					}
					else
					{
						(*ioCommands) [i] = command;

						expansions++;
					}
				}
			}
			else
			{
				(*ioCommands) [i] = command;

				expansions++;
			}
		}
	}
	
	if (success && (expansions > 0))
	{
		string	rebuiltCommandLine (RebuildCommandLine (*ioCommands));

		mInterface->WriteFormattedS ("%s\n", rebuiltCommandLine.c_str ());
	}

	return success;
}

// STATIC COMMAND HANDLERS

int
MRPShell::BPM (MRPShell *inShell)
{
	if (inShell->mParameters.size () == 1)
	{
		inShell->mInterface->WriteFormattedLF ("bpm is %lf\n", 
			inShell->mSequencer->GetBPM ());
	}
	else
	{
		float	bpm = atof (inShell->mParameters [1].c_str ());

		if (bpm > 0.0)
		{
			inShell->mSequencer->SetBPM (bpm);
		}
		else
		{
			inShell->mInterface->Write ("need bpm > 0");
		}
	}

	return 0;
}

int
MRPShell::ChangeChannel (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 3)
	{
		inShell->mInterface->Write ("need from channel/name and to channel/name\n");
		return 0;
	}

	int	fromPort (-1);
	int	fromChannel (0);

	if (inShell->mSequencer->GetNamedChannel
		(inShell->mParameters [1], &fromPort, &fromChannel))
	{
		// sweet
	}
	else
	{
		fromChannel = atoi (inShell->mParameters [1].c_str ());

		if (fromChannel < 1 || fromChannel > 16)
		{
			inShell->mInterface->Write ("bad from channel/name\n");
			return 0;
		}
	}

	int	toPort (-1);
	int	toChannel (0);

	if (inShell->mSequencer->GetNamedChannel
		(inShell->mParameters [2], &toPort, &toChannel))
	{
		// sweet
	}
	else
	{
		toChannel = atoi (inShell->mParameters [2].c_str ());

		if (toChannel < 1 || toChannel > 16)
		{
			inShell->mInterface->Write ("bad to channel/name\n");
			return 0;
		}
	}

	// -1 is a special port
	// saying "don't match" or "don't set"
	MRPSelectionChangeChannel	changer
		(fromPort, fromChannel, toPort, toChannel);

	int	selections = inShell->mSequencer->RunSelection (&changer);

	inShell->mInterface->WriteFormattedD ("changed channel on %d selections\n", selections);

	return 0;
}

int
MRPShell::ChangeNote (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 3)
	{
		inShell->mInterface->Write ("need from note/name and to note/name\n");
		return 0;
	}

	int	fromNote (-1);

	if (isdigit (inShell->mParameters [1][0]))
	{
		fromNote = atoi (inShell->mParameters [1].c_str ());
	}
	else
	{
		fromNote = MRPEvent::ParseNoteNumber (inShell->mParameters [1]);
	}

	if (fromNote < 0 || fromNote > 127)
	{
		inShell->mInterface->Write ("bad from note name/number\n");
		return 0;
	}

	int	toNote (-1);

	if (isdigit (inShell->mParameters [2][0]))
	{
		toNote = atoi (inShell->mParameters [2].c_str ());
	}
	else
	{
		toNote = MRPEvent::ParseNoteNumber (inShell->mParameters [2]);
	}

	if (toNote < 0 || toNote > 127)
	{
		inShell->mInterface->Write ("bad to note name/number\n");
		return 0;
	}

	MRPSelectionChangeNote	notiser (fromNote, toNote);

	int	selections = inShell->mSequencer->RunSelection (&notiser);

	string	noteName (MRPEvent::FormatNoteNumber (toNote));

	inShell->mInterface->WriteFormattedSD
		("set note %s on %d selections\n", noteName.c_str (), selections);

	return 0;
}
int
MRPShell::CopyTrack (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 3)
	{
		inShell->mInterface->Write ("need from track and to track\n");
		return 0;
	}

	int	from = inShell->GetTrackNumber (inShell->mParameters [1]);

	if (from < 1 || from > inShell->mSequencer->GetTrackCount ())
	{
		inShell->mInterface->Write ("bad from track number\n");
	}
	else
	{
		int	to = inShell->GetTrackNumber (inShell->mParameters [2]);

		if (to < 1 || to > inShell->mSequencer->GetTrackCount ())
		{
			inShell->mInterface->Write ("bad to track number\n");
		}
		else
		{
			inShell->mSequencer->CopyTrack (from, to);

			inShell->mInterface->WriteFormattedDD ("copied track %d to track %d\n", from, to);
		}
	}

	return 0;
}

int
MRPShell::ChangeDirectory (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need directory parameter\n");
	}
	else
	{
		int	errCode = chdir (inShell->mParameters [1].c_str ());

		if (errCode == -1)
		{
			inShell->mInterface->WriteFormattedS
				("cd failed: %s\n", sys_errlist [errno]);
		}
		else
		{
			char	wd [MAXPATHLEN];

			inShell->mInterface->WriteFormattedS
				("current directory now %s\n", getcwd (wd, sizeof (wd)));
		}
	}

	return 0;
}

int
MRPShell::ClearSelection (MRPShell *inShell)
{
	MRPSelectionClear	clear;

	inShell->mSequencer->RunSelection (&clear);

	inShell->mInterface->Write ("cleared selection\n");;

	return 0;
}

int
MRPShell::DefaultSequence (MRPShell *inShell)
{
	inShell->Execute ("load default.mrp");

	return 0;
}

int
MRPShell::DeleteConfig (MRPShell *inShell)
{
	inShell->mSequencer->DeleteChannelNames ();

	inShell->mInterface->Write ("deleted config\n");

	return 0;
}

int
MRPShell::DeleteSequence (MRPShell *inShell)
{
	while (inShell->mSequencer->GetTrackCount () > 0)
	{
		inShell->mSequencer->DeleteTrack (1);
	}

	inShell->mInterface->Write ("deleted sequence\n");

	return 0;
}

int
MRPShell::DeleteTrack (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need track number\n");
		return 0;
	}

	int	track = inShell->GetTrackNumber (inShell->mParameters [1]);

	if (track < 1 || track > inShell->mSequencer->GetTrackCount ())
	{
		inShell->mInterface->Write ("bad track number\n");
		return 0;
	}

	inShell->mSequencer->DeleteTrack (track);

	inShell->mInterface->WriteFormattedD ("deleted track %d\n", track);

	return 0;
}

int
MRPShell::DirectoryListing (MRPShell *inShell)
{
	string	command ("ls");

	if (inShell->mParameters.size () > 1)
	{
		command += " ";
		command += inShell->mParameters [1];
	}

	FILE	*fp = popen (command.c_str (), "r");

	if (fp != NULL)
	{
		char	buffer [128];

		while (fgets (buffer, 128, fp) != NULL)
		{
			inShell->mInterface->Write (buffer);
		}
	}

	return 0;
}

int
MRPShell::Display (MRPShell *inShell)
{
	// bpm

	inShell->mInterface->WriteFormattedLF
		("|BPM:%lf\n", inShell->mSequencer->GetBPM ());

	// divider

	inShell->mInterface->Write ("|---------|");

	for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
	{
		if (s >= inShell->mSequencer->GetLoopStep ())
		{
			inShell->mInterface->Write ("-X-|");
		}
		else
		{
			inShell->mInterface->Write ("---|");
		}
	}

	inShell->mInterface->Write ("\n");

	inShell->mInterface->Write ("|Track:|S:|");

	for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
	{
		inShell->mInterface->WriteFormattedD ("%03d|", s);
	}

	inShell->mInterface->Write ("\n");

	for (int t = 1; t <= inShell->mSequencer->GetTrackCount (); t++)
	{
		const MRPTrack	&track (inShell->mSequencer->GetTrack (t));

		// divider

		inShell->mInterface->Write ("|---------|");

		for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
		{
			if (inShell->mSequencer->GetEventStatus (t, s).IsSelected ())
			{
				inShell->mInterface->Write ("-*-|");
			}
			else
			{
				inShell->mInterface->Write ("---|");
			}
		}

		inShell->mInterface->Write ("\n");

		// type

		inShell->mInterface->Write ("|      |T:|");

		for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
		{
			const MRPEvent	&event (inShell->mSequencer->GetEvent (t, s));

			string	typeName;

			if (event.GetMessage () == MIDICLClient::kMIDINoteOffMessage)
			{
				typeName = "nof";
			}
			else if (event.GetMessage () == MIDICLClient::kMIDINoteOnMessage)
			{
				typeName = "not";
			}
			else if (event.GetMessage () == MIDICLClient::kMIDIPolyAftertouchMessage)
			{
				typeName = "ppr";
			}
			else if (event.GetMessage () == MIDICLClient::kMIDIControlChangeMessage)
			{
				typeName = "ctl";
			}
			else if (event.GetMessage () == MIDICLClient::kMIDIProgramChangeMessage)
			{
				typeName = "prg";
			}
			else if (event.GetMessage () == MIDICLClient::kMIDIChannelAftertouchMessage)
			{
				typeName = "cpr";
			}
			else if (event.GetMessage () == MIDICLClient::kMIDIPitchBendMessage)
			{
				typeName = "bnd";
			}
			else
			{
				typeName = "???";
			}

			inShell->mInterface->WriteFormattedS ("%s|", typeName.c_str ());
		}

		inShell->mInterface->Write ("\n");

		// port/channel

		inShell->mInterface->Write ("|      |C:|");

		for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
		{
			const MRPEvent	&event (inShell->mSequencer->GetEvent (t, s));

			string	channelName (inShell->mSequencer->GetChannelName
				(event.GetPort (), event.GetChannel ()));

			if (channelName.length () > 0)
			{
				if (channelName.length () < 3)
				{
					while (channelName.length () < 3)
					{
						channelName += ' ';
					}
				}
				else
				if (channelName.length () > 3)
				{
					channelName.erase (3);
				}

				inShell->mInterface->WriteFormattedS ("%s|", channelName.c_str ());
			}
			else
			{
				inShell->mInterface->WriteFormattedDD
					("%x%2d|", event.GetPort (), event.GetChannel ());
			}
		}

		inShell->mInterface->Write ("\n");

		// track name

		string	trackName (inShell->mSequencer->GetTrackName (t));

		if (trackName.length () == 0)
		{
			inShell->mInterface->WriteFormattedD ("|  %02d  ", t);
		}
		else
		{
			if (trackName.length () < 6)
			{
				trackName.append (6 - trackName.length (), ' ');
			}

			if (trackName.length () > 6)
			{
				trackName = trackName.substr (0, 6);
			}

			inShell->mInterface->WriteFormattedS ("|%s", trackName.c_str ());
		}

		// data1 (note,control,program,etc)

		inShell->mInterface->Write ("|D:|");

		for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
		{
			const MRPEvent	&event (inShell->mSequencer->GetEvent (t, s));

			if (event.GetMessage () == MIDICLClient::kMIDINoteOffMessage
				|| event.GetMessage () == MIDICLClient::kMIDINoteOnMessage
				|| event.GetMessage () == MIDICLClient::kMIDIPolyAftertouchMessage)
			{
				string	noteName (MRPEvent::FormatNoteNumber (event.GetData1 ()));

				if (noteName.length () < 3)
				{
					noteName.append (3 - noteName.length (), ' ');
				}
				else if (noteName.length () > 3)
				{
					noteName = noteName.substr (0, 3);
				}

				inShell->mInterface->WriteFormattedS ("%s|", noteName.c_str ());
			}
			else
			{
				inShell->mInterface->WriteFormattedD ("%3d|", event.GetData1 ());
			}
		}

		inShell->mInterface->Write ("\n");

		// data2 (velocity,pressure,etc)

		inShell->mInterface->Write ("|      |D:|");

		for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
		{
			const MRPEvent	&event (inShell->mSequencer->GetEvent (t, s));

			if (event.GetMessage () == MIDICLClient::kMIDIProgramChangeMessage
				|| event.GetMessage () == MIDICLClient::kMIDIChannelAftertouchMessage)
			{
				// no data2 for program change messages
				inShell->mInterface->Write ("   |");
			}
			else
			{
				inShell->mInterface->WriteFormattedD ("%3d|", event.GetData2 ());
			}
		}

		inShell->mInterface->Write ("\n");

		// track-mute

		if (track.IsMuted ())
		{
			inShell->mInterface->Write ("| (MU) ");
		}
		else
		{
			inShell->mInterface->Write ("|      ");
		}

		// gate

		inShell->mInterface->Write ("|G:|");

		for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
		{
			const MRPEvent	&event (inShell->mSequencer->GetEvent (t, s));

			// gate time only for note messages
			if (event.GetMessage () == MIDICLClient::kMIDINoteOffMessage
				|| event.GetMessage () == MIDICLClient::kMIDINoteOnMessage)
			{
				inShell->mInterface->WriteFormattedD ("%3d|", event.GetGateTime ());
			}
			else
			{
				inShell->mInterface->Write ("   |");
			}
		}

		inShell->mInterface->Write ("\n");

		// mute/hsolo/vsolo

		inShell->mInterface->Write ("|      |S:|");

		for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
		{
			const MRPEventStatus	&status
				(inShell->mSequencer->GetEventStatus (t, s));

			inShell->mInterface->Write (status.IsMuted () ? "M" : " ");
			inShell->mInterface->Write (status.IsHSoloed () ? "H" : " ");
			inShell->mInterface->Write (status.IsVSoloed () ? "V" : " ");

			inShell->mInterface->Write ("|");
		}

		inShell->mInterface->Write ("\n");
	}

	// divider

	inShell->mInterface->Write ("|---------|");

	for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
	{
		inShell->mInterface->Write ("---|");
	}

	inShell->mInterface->Write ("\n");

	// step actions

	for (int s = 0; s < inShell->mSequencer->GetStepCount (); s++)
	{
		bool	first (true);

		if (s < (int) inShell->mStepActions.size ())
		{
			string	stepAction (inShell->mStepActions [s]);

			if (stepAction.length () > 0)
			{
				if (first)
				{
					inShell->mInterface->Write ("step actions:\n");
					first = false;
				}

				inShell->mInterface->WriteFormattedD ("%d: ", s + 1);

				inShell->mInterface->WriteFormattedS
					("%s\n", stepAction.c_str ());
			}
		}
	}

	return 0;
}

int
MRPShell::ExportSMF (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need file name to export to\n");
		return 0;
	}

	if (inShell->mSequencer->GetTrackCount () == 0)
	{
		inShell->mInterface->Write ("nowt in memory to export\n");
		return 0;
	}

	// MRPSequencerSMF	smf (inShell->mSequencer, inShell->mInterface);

	// smf.Export (inShell->mParameters [1].c_str ());

	return 0;
}

int
MRPShell::FitToKey (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need key name or number\n");
		return 0;
	}

	int	key = -1;

	if (isdigit (inShell->mParameters [1][0]))
	{
		key = atoi (inShell->mParameters [1].c_str ());
		
		if (key < 0 || key >= MRPSelectionFitToKey::GetKeyNameCount ())
		{
			inShell->mInterface->Write ("bad key number\n");
			return 0;
		}
	}
	else
	{
		for (int i = 0; i < MRPSelectionFitToKey::GetKeyNameCount (); i++)
		{
			if (strcasecmp (inShell->mParameters [1].c_str (),
				(char *) MRPSelectionFitToKey::GetKeyName ((MRPSelectionFitToKey::Key) i)) == 0)
			{
				key = i;

				break;
			}
		}

		if (key == -1)
		{
			inShell->mInterface->Write ("bad key name\n");
			return 0;
		}
	}

	MRPSelectionFitToKey	fit ((MRPSelectionFitToKey::Key) key);

	int	selections = inShell->mSequencer->RunSelection (&fit);

	inShell->mInterface->WriteFormattedSD ("fit %s on %d selections\n",
		MRPSelectionFitToKey::GetKeyName ((MRPSelectionFitToKey::Key) key), selections);

	return 0;
}

int
MRPShell::Help (MRPShell *inShell)
{
	Version (inShell);

	inShell->mInterface->Write ("hint: (param1) means param is required\n");
	inShell->mInterface->Write ("hint: [param1] means param is optional\n");

	for (unsigned int i = 0; i < kNumCommands; i++)
	{
		inShell->mInterface->Write (sCommands [i].mCommand);

		if (sCommands [i].mAlias [0] != 0)
		{
			inShell->mInterface->Write ("/");
			inShell->mInterface->Write (sCommands [i].mAlias);
		}
		
		inShell->mInterface->WriteFormattedS
			(": %s\n", sCommands [i].mDescription);
	}
	
	return 0;
}

int
MRPShell::History (MRPShell *inShell)
{
	for (int i = 0; i < inShell->GetHistoryCount (); i++)
	{
		pair <int, string>	entry (inShell->GetHistoryEntryByIndex (i));

		inShell->mInterface->WriteFormattedD ("%d ", entry.first);
		inShell->mInterface->WriteFormattedS ("%s\n", entry.second.c_str ());
	}

	return 0;
}

int
MRPShell::HSolo (MRPShell *inShell)
{
	MRPSelectionHSolo	hsolo;

	int	selections = inShell->mSequencer->RunSelection (&hsolo);

	inShell->mInterface->WriteFormattedD ("h-soloed %d selections\n", selections);

	return 0;
}

int
MRPShell::HUnSolo (MRPShell *inShell)
{
	MRPSelectionHUnSolo	hunsolo;

	int	selections = inShell->mSequencer->RunSelection (&hunsolo);

	inShell->mInterface->WriteFormattedD ("h-unsoloed %d selections\n", selections);

	return 0;
}

int
MRPShell::ImportSMF (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need a file name argument\n");
		return 0;
	}

	// MRPSequencerSMF	smf (inShell->mSequencer, inShell->mInterface);

	// smf.Import (inShell->mParameters [1].c_str ());

	return 0;
}

int
MRPShell::InternalSync (MRPShell *inShell)
{
	inShell->mSequencer->SetInternalSync ();

	inShell->mInterface->Write ("using internal sync\n");

	return 0;
}

int
MRPShell::InvertMute (MRPShell *inShell)
{
	MRPSelectionInvertMute	invertMute;

	int	selections = inShell->mSequencer->RunSelection (&invertMute);

	inShell->mInterface->WriteFormattedD ("inverted mute status on %d selections\n", selections);

	return 0;
}

int
MRPShell::InvertSelection (MRPShell *inShell)
{
	for (int t = 1; t <= inShell->mSequencer->GetTrackCount (); t++)
	{
		for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
		{
			inShell->mSequencer->InvertSelect (t, s);
		}
	}

	inShell->mInterface->Write ("inverted selection\n");;

	return 0;
}

int
MRPShell::Load (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need file name to load from\n");
		return 0;
	}

	FILE	*file = fopen (inShell->mParameters [1].c_str (), "r");

	if (file == NULL)
	{
		inShell->mInterface->WriteFormattedS
			("can't open '%s'\n", inShell->mParameters [1].c_str ());
	}
	else
	{
		DeleteSequence (inShell);

		// spawn sub-shell
		MRPStdioInterface	interface (file, NULL);

		interface.SetOutputDelegate (inShell->mInterface);

		if (true)
		{
			MRPShell	shell (inShell->mSequencer, &interface);

			// migrate the migration mode :-)
			shell.mMigrationMode = inShell->mMigrationMode;

			shell.Shell (false);
		}

		fclose (file);
	}

	return 0;
}

int
MRPShell::MIDISync (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need port number\n");
		return 0;
	}

	unsigned long	port = 0;
	unsigned long	numPorts = inShell->mSequencer->GetInputPortCount ();

	sscanf (inShell->mParameters [1].c_str (), "%lu", &port);

	if (port >= numPorts)
	{
		inShell->mInterface->Write ("bad port number\n");
	}
	else
	{
		inShell->mSequencer->SetMIDISync (port);

		inShell->mInterface->WriteFormattedD ("using midi sync from port %d\n", port);
	}

	return 0;
}

int
MRPShell::Migration (MRPShell *inShell)
{
	if (inShell->mParameters.size () == 1)
	{
		inShell->mMigrationMode = !inShell->mMigrationMode;
	}
	else
	{
		inShell->mMigrationMode
			= (strcasecmp (inShell->mParameters [1].c_str (), "on") == 0);
	}

	inShell->mInterface->WriteFormattedS
		("migration mode %s\n", inShell->mMigrationMode ? "on" : "off");

	return 0;
}

int
MRPShell::Mute (MRPShell *inShell)
{
	MRPSelectionMute	mute;

	int	selections = inShell->mSequencer->RunSelection (&mute);

	inShell->mInterface->WriteFormattedD ("muted %d selections\n", selections);

	return 0;
}

int
MRPShell::MuteTrack (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need track number\n");
		return 0;
	}

	int	track = inShell->GetTrackNumber (inShell->mParameters [1]);

	if (track < 1 || track > inShell->mSequencer->GetTrackCount ())
	{
		inShell->mInterface->Write ("bad track number\n");
		return 0;
	}

	inShell->mSequencer->MuteTrack (track);

	inShell->mInterface->WriteFormattedD ("muted track %d\n", track);

	return 0;
}

int
MRPShell::NameChannel (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 3)
	{
		inShell->mInterface->Write ("need channel, or port and channel\n");
		return 0;
	}

	int	port = 0;
	int channel = 0;
	string	name;

	if (inShell->mParameters.size () > 3)
	{
		port = atoi (inShell->mParameters [1].c_str ());
		channel = atoi (inShell->mParameters [2].c_str ());
		name = inShell->mParameters [3];
	}
	else
	{
		channel = atoi (inShell->mParameters [1].c_str ());
		name = inShell->mParameters [2];
	}

	if (port < 0 || port >= inShell->mSequencer->GetOutputPortCount ())
	{
		inShell->mInterface->Write ("bad port number\n");
		return 0;
	}

	if (channel < 1 || channel > 16)
	{
		inShell->mInterface->Write ("bad channel number\n");
		return 0;
	}

	inShell->mSequencer->SetChannelName (port, channel, name);

	inShell->mInterface->WriteFormattedDD ("port %d channel %d named ", port, channel);
	inShell->mInterface->WriteFormattedS ("'%s'\n", name.c_str ());

	return 0;
}

int
MRPShell::NameTrack (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need at least track number\n");
		return 0;
	}

	int	track = inShell->GetTrackNumber (inShell->mParameters [1]);

	if (track == -1)
	{
		inShell->mInterface->Write ("bad track name/number\n");
		return 0;
	}

	if (inShell->mParameters.size () > 2)
	{
		inShell->mSequencer->SetTrackName (track, inShell->mParameters [2]);

		inShell->mInterface->WriteFormattedD ("track %d named ", track);
		inShell->mInterface->WriteFormattedS ("'%s'\n", inShell->mParameters [2].c_str ());
	}
	else
	{
		inShell->mSequencer->SetTrackName (track, "");

		inShell->mInterface->WriteFormattedD ("track %d name deleted\n", track);
	}


	return 0;
}

int
MRPShell::NewTrack (MRPShell *inShell)
{
	int	track = -1;
	MRPTrack	trackObject;

	if (inShell->mParameters.size () < 2)
	{
		track = inShell->mSequencer->AddTrack (trackObject);
	}
	else
	{
		if (isdigit (inShell->mParameters [1][0]))
		{
			track = atoi (inShell->mParameters [1].c_str ());

			if (track < 1)
			{
				inShell->mInterface->Write ("bad track number\n");
			}
			else
			{
				inShell->mInterface->WriteFormattedD
					("adding track %d\n", track);

				if (inShell->mMigrationMode)
				{
					inShell->mInterface->Write
						("migration mode: bumping track number\n");
					track++;
				}

				track = inShell->mSequencer->AddTrack (trackObject, track);
			}
		}
		else
		{
			inShell->mInterface->Write ("bad track number\n");
		}
	}

	if (track <= 0)
	{
		inShell->mInterface->Write ("couldn't add track\n");
	}
	else
	{
		inShell->mInterface->WriteFormattedD ("added track %d\n", track);
	}

	return 0;
}

int
MRPShell::Pause (MRPShell *inShell)
{
	inShell->mSequencer->Stop ();

	return 0;
}

int
MRPShell::Play (MRPShell *inShell)
{
	inShell->mSequencer->Play ();

	return 0;
}

int
MRPShell::Ports (MRPShell *inShell)
{
	int numPorts = inShell->mSequencer->GetInputPortCount ();

	inShell->mInterface->WriteFormattedD ("found %d inputs: \n", numPorts);

	for (int i = 0; i < numPorts; i++)
	{
		char	displayName [64];
		char	name [64];
		char	manufacturer [64];
		char	model [64];

    displayName [0] = 0;
		name [0] = 0;
		manufacturer [0] = 0;
		model [0] = 0;

		CFStringRef	property = NULL;

		if (MIDIObjectGetStringProperty
			(MIDIGetSource (i), kMIDIPropertyName, &property) == 0)
		{
			CFStringGetCString (property, name, sizeof (name), 0);
			CFRelease (property);
			property = NULL;
		}

		if (MIDIObjectGetStringProperty
			(MIDIGetSource (i), kMIDIPropertyDisplayName, &property) == 0)
		{
			CFStringGetCString (property, displayName, sizeof (displayName), 0);
			CFRelease (property);
			property = NULL;
		}

		if (MIDIObjectGetStringProperty
			(MIDIGetSource (i), kMIDIPropertyManufacturer, &property) == 0)
		{
			CFStringGetCString
				(property, manufacturer, sizeof (manufacturer), 0);
			CFRelease (property);
			property = NULL;
		}

		if (MIDIObjectGetStringProperty
			(MIDIGetSource (i), kMIDIPropertyModel, &property) == 0)
		{
			CFStringGetCString (property, model, sizeof (model), 0);
			CFRelease (property);
			property = NULL;
		}

		inShell->mInterface->WriteFormattedD ("%d ", i);

		if (strlen(displayName) > 0)
		{
      inShell->mInterface->WriteFormattedS ("dn=%s ", displayName);
    }

		if (strlen(name) > 0)
		{
      inShell->mInterface->WriteFormattedS ("n=%s ", name);
    }

		if (strlen(manufacturer) > 0)
		{
      inShell->mInterface->WriteFormattedS ("mfr=%s ", manufacturer);
    }

		if (strlen(model) > 0)
		{
      inShell->mInterface->WriteFormattedS ("model=%s", model);
    }

    inShell->mInterface->WriteFormattedS ("\n", model);
	}

	numPorts = inShell->mSequencer->GetOutputPortCount ();

	inShell->mInterface->WriteFormattedD ("found %d outputs: \n", numPorts);

	for (int i = 0; i < numPorts; i++)
	{
		char	displayName [64];
		char	name [64];
		char	manufacturer [64];
		char	model [64];

    displayName [0] = 0;
		name [0] = 0;
		manufacturer [0] = 0;
		model [0] = 0;

		CFStringRef	property = NULL;

		if (MIDIObjectGetStringProperty
			(MIDIGetDestination (i), kMIDIPropertyDisplayName, &property) == 0)
		{
			CFStringGetCString (property, displayName, sizeof (displayName), 0);
			CFRelease (property);
		}

		if (MIDIObjectGetStringProperty
			(MIDIGetDestination (i), kMIDIPropertyName, &property) == 0)
		{
			CFStringGetCString (property, name, sizeof (name), 0);
			CFRelease (property);
		}

		if (MIDIObjectGetStringProperty
			(MIDIGetDestination (i), kMIDIPropertyManufacturer, &property) == 0)
		{
			CFStringGetCString (property, manufacturer, sizeof (manufacturer), 0);
			CFRelease (property);
		}

		if (MIDIObjectGetStringProperty
			(MIDIGetDestination (i), kMIDIPropertyModel, &property) == 0)
		{
			CFStringGetCString (property, model, sizeof (model), 0);
			CFRelease (property);
		}

		inShell->mInterface->WriteFormattedD ("%d ", i);
		
		if (strlen(displayName) > 0)
		{
      inShell->mInterface->WriteFormattedS ("dn=%s ", displayName);
    }

		if (strlen(name) > 0)
		{
      inShell->mInterface->WriteFormattedS ("n=%s ", name);
    }

		if (strlen(manufacturer) > 0)
		{
      inShell->mInterface->WriteFormattedS ("mfr=%s ", manufacturer);
    }

		if (strlen(model) > 0)
		{
      inShell->mInterface->WriteFormattedS ("model=%s", model);
    }

    inShell->mInterface->WriteFormattedS ("\n", model);
	}

	return 0;
}

int
MRPShell::Quit (MRPShell *inShell)
{
	return 1;
}

int
MRPShell::RandomiseChannel (MRPShell *inShell)
{
	int	base = 1;
	int	range = 15;

	if (inShell->mParameters.size () > 1)
	{
		base = atoi (inShell->mParameters [1].c_str ());

		if (inShell->mParameters.size () > 2)
		{
			range = atoi (inShell->mParameters [2].c_str ());
		}
	}

	if (base < 1 || base > 16)
	{
		inShell->mInterface->Write ("channel must be 1-16\n");
		return 0;
	}

	if (range < 0 || range > 15)
	{
		inShell->mInterface->Write ("range must be 0-15\n");
		return 0;
	}

	if (range == 0)
	{
		range = 16 - base;
	}

	if ((base + range) > 16)
	{
		inShell->mInterface->Write ("base + range must be 1-16\n");
		return 0;
	}

	MRPSelectionRandomiseChannel	randomiser (base, range);

	int	selections = inShell->mSequencer->RunSelection (&randomiser);

	inShell->mInterface->WriteFormattedD
		("randomised channel on %d selections\n", selections);

	inShell->mInterface->WriteFormattedDD
		("base channel %d range %d channels\n", base, range);

	return 0;
}

int
MRPShell::RandomiseGate (MRPShell *inShell)
{
	int	base = 0;
	int	range = 90;

	if (inShell->mParameters.size () > 1)
	{
		base = atoi (inShell->mParameters [1].c_str ());

		if (inShell->mParameters.size () > 2)
		{
			range = atoi (inShell->mParameters [2].c_str ());
		}
	}

	if (base < 0 || base > 90)
	{
		inShell->mInterface->Write ("base must be 0-90\n");
		return 0;
	}

	if (range < 0 || range > 90)
	{
		inShell->mInterface->Write ("range must be 0-90\n");
		return 0;
	}

	if (range == 0)
	{
		range = 90 - base;
	}

	if ((base + range) > 90)
	{
		inShell->mInterface->Write ("base + range must be 0-90\n");
		return 0;
	}

	MRPSelectionRandomiseGate	randomiser (base, range);

	int	selections = inShell->mSequencer->RunSelection (&randomiser);

	inShell->mInterface->WriteFormattedD
		("randomised gate on %d selections\n", selections);

	inShell->mInterface->WriteFormattedDD
		("base gate %d range %d%%\n", base, range);

	return 0;
}

int
MRPShell::RandomiseMute (MRPShell *inShell)
{
	MRPSelectionRandomMute	randomiser;

	int	selections = inShell->mSequencer->RunSelection (&randomiser);

	inShell->mInterface->WriteFormattedD ("randomised mute on %d selections\n", selections);

	return 0;
}

int
MRPShell::RandomiseNote (MRPShell *inShell)
{
	int	base = 0;
	int	range = 127;

	if (inShell->mParameters.size () > 1)
	{
		if (isdigit (inShell->mParameters [1][0]))
		{
			base = atoi (inShell->mParameters [1].c_str ());
		}
		else
		{
			base = MRPEvent::ParseNoteNumber (inShell->mParameters [1]);
		}

		if (inShell->mParameters.size () > 2)
		{
			range = atoi (inShell->mParameters [2].c_str ());
		}
	}

	if (base < 0 || base > 127)
	{
		inShell->mInterface->Write ("note must be 0-127\n");
		return 0;
	}

	if (range < 0 || range > 127)
	{
		inShell->mInterface->Write ("range must be 0-127\n");
		return 0;
	}

	if (range == 0)
	{
		range = 127 - base;
	}

	if ((base + range) > 127)
	{
		inShell->mInterface->Write ("base + range must be 0-127\n");
		return 0;
	}

	MRPSelectionRandomiseNote	randomiser (base, range);

	int	selections = inShell->mSequencer->RunSelection (&randomiser);

	inShell->mInterface->WriteFormattedD
		("randomised note on %d selections\n", selections);

	inShell->mInterface->WriteFormattedSD
		("base note %s range %d note\n", MRPEvent::FormatNoteNumber (base).c_str (), range);

	return 0;
}

int
MRPShell::RandomisePort (MRPShell *inShell)
{
	if (inShell->mSequencer->GetOutputPortCount () == 0)
	{
		return 0;
	}

	int	portRange (inShell->mSequencer->GetOutputPortCount () - 1);

	int	base = 0;
	int	range = portRange;

	if (inShell->mParameters.size () > 1)
	{
		base = atoi (inShell->mParameters [1].c_str ());

		if (inShell->mParameters.size () > 2)
		{
			range = atoi (inShell->mParameters [2].c_str ());
		}
	}

	if (base < 0 || base > portRange)
	{
		inShell->mInterface->WriteFormattedD ("base must be 0-%d\n", portRange);
		return 0;
	}

	if (range < 0 || range > portRange)
	{
		inShell->mInterface->WriteFormattedD ("range must be 0-%d\n", portRange);
		return 0;
	}

	if (range == 0)
	{
		range = portRange - base;
	}

	if ((base + range) > portRange)
	{
		inShell->mInterface->WriteFormattedD ("base + range must be 0-%d\n", portRange);
		return 0;
	}

	MRPSelectionRandomisePort	randomiser (base, range);

	int	selections = inShell->mSequencer->RunSelection (&randomiser);

	inShell->mInterface->WriteFormattedD
		("randomised port on %d selections\n", selections);

	inShell->mInterface->WriteFormattedDD
		("base port %d range %d ports\n", base, range);

	return 0;
}

int
MRPShell::RandomiseData1 (MRPShell *inShell)
{
	int	base = 0;
	int	range = 127;

	if (inShell->mParameters.size () > 1)
	{
		base = atoi (inShell->mParameters [1].c_str ());

		if (inShell->mParameters.size () > 2)
		{
			range = atoi (inShell->mParameters [2].c_str ());
		}
	}

	if (base < 0 || base > 127)
	{
		inShell->mInterface->Write ("base must be 0-127\n");
		return 0;
	}

	if (range < 0 || range > 127)
	{
		inShell->mInterface->Write ("range must be 0-127\n");
		return 0;
	}

	if (range == 0)
	{
		range = 127 - base;
	}

	if ((base + range) > 127)
	{
		inShell->mInterface->Write ("base + range must be 0-127\n");
		return 0;
	}

	MRPSelectionRandomiseData1	randomiser (base, range);

	int	selections = inShell->mSequencer->RunSelection (&randomiser);

	inShell->mInterface->WriteFormattedD
		("randomised data1 on %d selections\n", selections);

	inShell->mInterface->WriteFormattedDD
		("base data1 %d range %d\n", base, range);

	return 0;
}

int
MRPShell::RandomiseData2 (MRPShell *inShell)
{
	int	base = 0;
	int	range = 127;

	if (inShell->mParameters.size () > 1)
	{
		base = atoi (inShell->mParameters [1].c_str ());

		if (inShell->mParameters.size () > 2)
		{
			range = atoi (inShell->mParameters [2].c_str ());
		}
	}

	if (base < 0 || base > 127)
	{
		inShell->mInterface->Write ("base must be 0-127\n");
		return 0;
	}

	if (range < 0 || range > 127)
	{
		inShell->mInterface->Write ("range must be 0-127\n");
		return 0;
	}

	if (range == 0)
	{
		range = 127 - base;
	}

	if ((base + range) > 127)
	{
		inShell->mInterface->Write ("base + range must be 0-127\n");
		return 0;
	}

	MRPSelectionRandomiseData2	randomiser (base, range);

	int	selections = inShell->mSequencer->RunSelection (&randomiser);

	inShell->mInterface->WriteFormattedD
		("randomised data2 on %d selections\n", selections);

	inShell->mInterface->WriteFormattedDD
		("base data2 %d range %d\n", base, range);

	return 0;
}

int
MRPShell::RandomiseVelocity (MRPShell *inShell)
{
	int	base = 0;
	int	range = 127;

	if (inShell->mParameters.size () > 1)
	{
		base = atoi (inShell->mParameters [1].c_str ());

		if (inShell->mParameters.size () > 2)
		{
			range = atoi (inShell->mParameters [2].c_str ());
		}
	}

	if (base < 0 || base > 127)
	{
		inShell->mInterface->Write ("base must be 0-127\n");
		return 0;
	}

	if (range < 0 || range > 127)
	{
		inShell->mInterface->Write ("range must be 0-127\n");
		return 0;
	}

	if (range == 0)
	{
		range = 127 - base;
	}

	if ((base + range) > 127)
	{
		inShell->mInterface->Write ("base + range must be 0-127\n");
		return 0;
	}

	MRPSelectionRandomiseVelocity	randomiser (base, range);

	int	selections = inShell->mSequencer->RunSelection (&randomiser);

	inShell->mInterface->WriteFormattedD
		("randomised velocity on %d selections\n", selections);

	inShell->mInterface->WriteFormattedDD
		("base velocity %d range %d\n", base, range);

	return 0;
}

int
MRPShell::Read (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need file name to load from\n");
		return 0;
	}

	FILE	*file = fopen (inShell->mParameters [1].c_str (), "r");

	if (file == NULL)
	{
		inShell->mInterface->WriteFormattedS
			("can't open '%s'\n", inShell->mParameters [1].c_str ());
	}
	else
	{
		// arrange I/O for the sub-shell
		MRPStdioInterface	interface (file, NULL);

		interface.SetOutputDelegate (inShell->mInterface);

		// spawn sub-shell
		MRPShell	shell (inShell->mSequencer, &interface);

		// migrate the migration mode :-)
		shell.mMigrationMode = inShell->mMigrationMode;

		shell.Shell (false);

		fclose (file);
	}

	return 0;
}

int
MRPShell::Reset (MRPShell *inShell)
{
	inShell->mSequencer->Reset ();

	return 0;
}

int
MRPShell::Rotate (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need rotation count\n");
		return 0;
	}

	int	count = atoi (inShell->mParameters [1].c_str ());

	int	selections (0);
	
	if (count >= 0)
	{
		selections = inShell->mSequencer->RotateSelectionForwards (count);
	}
	else
	{
		selections = inShell->mSequencer->RotateSelectionBackwards (count);
	}

	inShell->mInterface->WriteFormattedDD
		("rotated %d selections %d times\n", selections, count);

	return 0;
}

int
MRPShell::Save (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need file name to save to\n");
		return 0;
	}

	if (inShell->mConfirmSaves)
	{
		if (access (inShell->mParameters [1].c_str (), R_OK) == 0)
		{
			inShell->mInterface->WriteFormattedS
				("%s exists - overwrite (y/n) ?", inShell->mParameters [1].c_str ());

			int	length = 0;
			char	buffer [4];
			
			do
			{
				length = inShell->mInterface->ReadLine (buffer, sizeof (buffer));
			}
			while (length == 0);

			if (buffer [0] != 'y' && buffer [0] != 'Y')
			{
				return 0;
			}
		}
	}

	inShell->mInterface->Write ("opening file\n");

	FILE	*file = fopen (inShell->mParameters [1].c_str (), "w");

	if (file == NULL)
	{
		inShell->mInterface->WriteFormattedS
			("can't open '%s'\n", inShell->mParameters [1].c_str ());
	}
	else
	{
		inShell->mInterface->Write ("writing scale & loop\n");

		fprintf (file, "setscale %d %d\n",
			inShell->mSequencer->GetBarCount (), inShell->mSequencer->GetStepCount ());

		fprintf (file, "setloop %d\n", inShell->mSequencer->GetLoopStep ());

		inShell->mInterface->Write ("writing bpm\n");

		fprintf (file, "bpm %lf\n", inShell->mSequencer->GetBPM ());

		for (int t = 1; t <= inShell->mSequencer->GetTrackCount (); t++)
		{
			inShell->mInterface->WriteFormattedD ("writing track %d\n", t);

			// have to quote the track number
			// otherwise the following commands will address the wrong track (most likely)
			fprintf (file, "newtrack %d\n", t);

			string	trackName (inShell->mSequencer->GetTrackName (t));

			if (trackName.length () > 0)
			{
				fprintf (file, "nametrack %d '%s'\n", t, trackName.c_str ());
			}

			MRPTrack	&track (inShell->mSequencer->GetTrack (t));

			if (track.IsMuted ())
			{
				fprintf (file, "mutetrack %d\n", t);
			}

			for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
			{
				const MRPEvent	&event (inShell->mSequencer->GetEvent (t, s));
				const MRPEventStatus	&status
					(inShell->mSequencer->GetEventStatus (t, s));

				// set selection
				fprintf (file, "selectevent %d %d\n", t, s);

				unsigned char	message (event.GetType ());

				if (message == MIDICLClient::kMIDINoteOffMessage
					|| message == MIDICLClient::kMIDINoteOnMessage)
				{
					fprintf (file, "settype note\n");
				}
				else
				if (message == MIDICLClient::kMIDIPolyAftertouchMessage)
				{
					fprintf (file, "settype polypressure\n");
				}
				else
				if (message == MIDICLClient::kMIDIControlChangeMessage)
				{
					fprintf (file, "settype control\n");
				}
				else
				if (message == MIDICLClient::kMIDIProgramChangeMessage)
				{
					fprintf (file, "settype program\n");
				}
				else
				if (message == MIDICLClient::kMIDIChannelAftertouchMessage)
				{
					fprintf (file, "settype pressure\n");
				}
				else
				if (message == MIDICLClient::kMIDIPitchBendMessage)
				{
					fprintf (file, "settype pitchbend\n");
				}
				else
				{
					inShell->mInterface->WriteFormattedD
						("bad step type %02x in save\n", (int) message);
				}

				// set event attributes
				fprintf (file, "setdata1 %d\n", event.GetData1 ());
				fprintf (file, "setchannel %d\n", event.GetChannel ());
				fprintf (file, "setport %d\n", event.GetPort ());
				fprintf (file, "setgatetime %d\n", event.GetGateTime ());
				fprintf (file, "setdata2 %d\n", event.GetData2 ());

				if (status.IsMuted ())
				{
					fprintf (file, "mute\n");
				}

				if (status.IsHSoloed ())
				{
					fprintf (file, "hsolo\n");
				}

				if (status.IsVSoloed ())
				{
					fprintf (file, "vsolo\n");
				}

				// clear selection
				fprintf (file, "clear\n");
			}
		}
		
		// preserve selection

		inShell->mInterface->Write ("writing selection\n");

		for (int t = 1; t <= inShell->mSequencer->GetTrackCount (); t++)
		{
			for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
			{
				const MRPEventStatus	&status
					(inShell->mSequencer->GetEventStatus (t, s));

				if (status.IsSelected ())
				{
					fprintf (file, "selectevent %d %d\n", t, s);
				}
			}
		}

		inShell->mInterface->Write ("closing file\n");

		fclose (file);
	}

	return 0;
}

int
MRPShell::SaveConfig (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need file name to save to\n");
		return 0;
	}

	if (inShell->mConfirmSaves)
	{
		if (access (inShell->mParameters [1].c_str (), R_OK) == 0)
		{
			inShell->mInterface->WriteFormattedS
				("%s exists - overwrite (y/n) ?", inShell->mParameters [1].c_str ());

			int	length = 0;
			char	buffer [4];
			
			do
			{
				length = inShell->mInterface->ReadLine (buffer, sizeof (buffer));
			}
			while (length == 0);

			if (buffer [0] != 'y' && buffer [0] != 'Y')
			{
				return 0;
			}
		}
	}

	inShell->mInterface->Write ("opening file\n");

	FILE	*file = fopen (inShell->mParameters [1].c_str (), "w");

	if (file == NULL)
	{
		inShell->mInterface->WriteFormattedS
			("can't open '%s'\n", inShell->mParameters [1].c_str ());
	}
	else
	{
		inShell->mInterface->Write ("writing channel names\n");

		// channel names
		for (int p = 0; p < inShell->mSequencer->GetOutputPortCount (); p++)
		{
			for (int c = 0; c < 16; c++)
			{
				string	name (inShell->mSequencer->GetChannelName (p, c));

				if (name.length () > 0)
				{
					fprintf (file, "namechannel %d %d '%s'\n", p, c, name.c_str ());
				}
			}
		}

		inShell->mInterface->Write ("closing file\n");

		fclose (file);
	}

	return 0;
}

int
MRPShell::SaveSelection (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need file name to save to\n");
		return 0;
	}

	if (inShell->mConfirmSaves)
	{
		if (access (inShell->mParameters [1].c_str (), R_OK) == 0)
		{
			inShell->mInterface->WriteFormattedS
				("%s exists - overwrite (y/n) ?", inShell->mParameters [1].c_str ());

			int	length = 0;
			char	buffer [4];
			
			do
			{
				length = inShell->mInterface->ReadLine (buffer, sizeof (buffer));
			}
			while (length == 0);

			if (buffer [0] != 'y' && buffer [0] != 'Y')
			{
				return 0;
			}
		}
	}

	FILE	*file = fopen (inShell->mParameters [1].c_str (), "w");

	if (file == NULL)
	{
		inShell->mInterface->WriteFormattedS
			("can't open '%s'\n", inShell->mParameters [1].c_str ());
	}
	else
	{
		for (int t = 1; t <= inShell->mSequencer->GetTrackCount (); t++)
		{
			for (int s = 1; s <= inShell->mSequencer->GetStepCount (); s++)
			{
				const MRPEventStatus	&status
					(inShell->mSequencer->GetEventStatus (t, s));

				if (status.IsSelected ())
				{
					fprintf (file, "selectevent %d %d\n", t, s);
				}
			}
		}

		fclose (file);
	}

	return 0;
}

int
MRPShell::SelectAll (MRPShell *inShell)
{
	MRPSelectionSelectByClockDivision	selector (8);

	int	selections = inShell->mSequencer->SelectAll ();

	inShell->mInterface->WriteFormattedD
		("selected %d events\n", selections);

	return 0;
}

int
MRPShell::SelectEighthNotes (MRPShell *inShell)
{
	MRPSelectionSelectByClockDivision	selector (8);

	int	selections = inShell->mSequencer->RunSelection (&selector);

	inShell->mInterface->WriteFormattedD ("selected eighth notes within %d selections\n", selections);

	return 0;
}

int
MRPShell::SelectEvent (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 3)
	{
		inShell->mInterface->Write ("need track & step param\n");
		return 0;
	}

	int	track = inShell->GetTrackNumber (inShell->mParameters [1]);

	int	step = atoi (inShell->mParameters [2].c_str ());

	if (inShell->mMigrationMode)
	{
		inShell->mInterface->Write ("migration mode: bumping track & step numbers\n");

		track++;
		step++;
	}

	if (track < 1 || track > inShell->mSequencer->GetTrackCount ())
	{
		inShell->mInterface->Write ("bad track number\n");
	}
	else
	if (step < 1 || step > inShell->mSequencer->GetStepCount ())
	{
		inShell->mInterface->WriteFormattedD ("bad step number %d\n", step);
	}
	else
	{
		inShell->mSequencer->SelectEvent (track, step);

		inShell->mInterface->WriteFormattedDD ("selected track %d step %d\n", track, step);
	}

	return 0;
}

int
MRPShell::SelectQuarterNotes (MRPShell *inShell)
{
	MRPSelectionSelectByClockDivision	selector (4);

	int	selections = inShell->mSequencer->RunSelection (&selector);

	inShell->mInterface->WriteFormattedD ("selected quarter notes within %d selections\n", selections);

	return 0;
}

int
MRPShell::SelectStep (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need step param\n");
		return 0;
	}

	int	step = atoi (inShell->mParameters [1].c_str ());

	if (step < 1 || step > inShell->mSequencer->GetStepCount ())
	{
		inShell->mInterface->Write ("bad step number\n");
	}
	else
	{
		inShell->mSequencer->SelectStep (step);

		inShell->mInterface->WriteFormattedD ("selected step %d\n", step);
	}

	return 0;
}

int
MRPShell::SelectTrack (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need track param\n");
		return 0;
	}

	int	track = inShell->GetTrackNumber (inShell->mParameters [1]);

	if (track < 1 || track > inShell->mSequencer->GetTrackCount ())
	{
		inShell->mInterface->Write ("bad track number\n");
	}
	else
	{
		inShell->mSequencer->SelectTrack (track);

		inShell->mInterface->WriteFormattedD ("selected track %d\n", track);
	}

	return 0;
}

int
MRPShell::SetChannel (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need channel/name\n");
		return 0;
	}

	int	port (0);
	int	channel (0);

	if (inShell->mSequencer->GetNamedChannel (inShell->mParameters [1], &port, &channel))
	{
		MRPSelectionSetPort	portiser (port);

		inShell->mSequencer->RunSelection (&portiser);

		MRPSelectionSetChannel	channeliser (channel);

		int	selections = inShell->mSequencer->RunSelection (&channeliser);

		inShell->mInterface->WriteFormattedSD ("set port/channel %s on %d selections\n",
			inShell->mParameters [1].c_str (), selections);
	}
	else
	{
		channel = atoi (inShell->mParameters [1].c_str ());

		if (channel < 1 || channel > 16)
		{
			inShell->mInterface->Write ("need channel 1-16\n");
		}
		else
		{
			MRPSelectionSetChannel	channeliser (channel);

			int	selections = inShell->mSequencer->RunSelection (&channeliser);

			inShell->mInterface->WriteFormattedDD ("set channel %d on %d selections\n", channel, selections);
		}
	}

	return 0;
}

int
MRPShell::SetData1 (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need data1 0-127\n");
		return 0;
	}

	int	data1 = atoi (inShell->mParameters [1].c_str ());

	if (data1 < 0 || data1 > 127)
	{
		inShell->mInterface->Write ("need data1 0-127\n");
	}
	else
	{
		MRPSelectionSetData1	dater (data1);

		int	selections = inShell->mSequencer->RunSelection (&dater);

		inShell->mInterface->WriteFormattedDD
			("set data1 %d on %d selections\n", data1, selections);
	}

	return 0;
}

int
MRPShell::SetData2 (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need data2 0-127\n");
		return 0;
	}

	int	data2 = atoi (inShell->mParameters [1].c_str ());

	if (data2 < 0 || data2 > 127)
	{
		inShell->mInterface->Write ("need data2 0-127\n");
	}
	else
	{
		MRPSelectionSetData2	dater (data2);

		int	selections = inShell->mSequencer->RunSelection (&dater);

		inShell->mInterface->WriteFormattedDD
			("set data2 %d on %d selections\n", data2, selections);
	}

	return 0;
}

int
MRPShell::SetGateTime (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need gate percent param\n");
		return 0;
	}

	int	gate = atoi (inShell->mParameters [1].c_str ());

	if (gate < 0 || gate > 90)
	{
		inShell->mInterface->Write ("need gate 0-90\n");
	}
	else
	{
		MRPSelectionSetGateTime	gater (gate);

		int	selections = inShell->mSequencer->RunSelection (&gater);

		inShell->mInterface->WriteFormattedDD ("set gate %d%% on %d selections\n", gate, selections);
	}

	return 0;
}

int
MRPShell::SetPort (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need port param\n");
		return 0;
	}

	unsigned long	port = 0;
	unsigned long	numPorts = inShell->mSequencer->GetOutputPortCount ();

	sscanf (inShell->mParameters [1].c_str (), "%lu", &port);

	if (port >= numPorts)
	{
		inShell->mInterface->Write ("bad port number\n");
	}
	else
	{
		MRPSelectionSetPort	porter (port);

		int	selections = inShell->mSequencer->RunSelection (&porter);

		inShell->mInterface->WriteFormattedLUD ("set port %lu on %d selections\n", port, selections);
	}

	return 0;
}

int
MRPShell::SetLoop (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need loop step param\n");
		return 0;
	}

	if (! isdigit (inShell->mParameters [1][0]))
	{
		inShell->mInterface->Write ("bad step number\n");
		return 0;
	}

	int	step = atoi (inShell->mParameters [1].c_str ());

	// setloop(numSteps+1) == loop at end of sequence
	if (step < 1 || step > (inShell->mSequencer->GetStepCount () + 1))
	{
		inShell->mInterface->Write ("bad step number\n");
		return 0;
	}

	inShell->mSequencer->SetLoopStep (step);

	inShell->mInterface->WriteFormattedD ("set loop step to %d\n", step);

	return 0;
}

int
MRPShell::SetNote (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need note param\n");
		return 0;
	}

	int	note (-1);

	if (isdigit (inShell->mParameters [1][0]))
	{
		note = atoi (inShell->mParameters [1].c_str ());
	}
	else
	{
		note = MRPEvent::ParseNoteNumber (inShell->mParameters [1]);
	}

	if (note < 0 || note > 127)
	{
		inShell->mInterface->Write ("bad note name/number\n");
	}
	else
	{
		string	noteName (MRPEvent::FormatNoteNumber (note));

		MRPSelectionSetNote	notiser (note);

		int	selections = inShell->mSequencer->RunSelection (&notiser);

		inShell->mInterface->WriteFormattedSD ("set note %s (%d) ",
			noteName.c_str (), note);

		inShell->mInterface->WriteFormattedD ("on %d selections\n", selections);
	}

	return 0;
}

int
MRPShell::SetScale (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need bars & steps params\n");
		return 0;
	}

	int	bars = atoi (inShell->mParameters [1].c_str ());
	int	steps = atoi (inShell->mParameters [2].c_str ());

	if (bars <= 0 || steps <= 0)
	{
		inShell->mInterface->Write ("need sensible bars & steps\n");
	}
	else
	if (steps > 64)
	{
		inShell->mInterface->Write ("too many steps (max=64)\n");
	}
	else
	{
		inShell->mSequencer->SetScale (bars, steps);

		inShell->mInterface->WriteFormattedDD ("set scale to %d bars %d steps\n", bars, steps);
	}

	return 0;
}

int
MRPShell::SetStepAction (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need at least step number\n");
		return 0;
	}

	int	step1 = atoi (inShell->mParameters [1].c_str ());
	
	if (step1 < 1 || step1 > inShell->mSequencer->GetStepCount ())
	{
		inShell->mInterface->Write ("bad step number\n");
	}
	else
	{
		// accumulate any parameters into the command line
		string	commandLine;

		for (unsigned int i = 2; i < inShell->mParameters.size (); i++)
		{
			commandLine += inShell->mParameters [i];
			commandLine += ' ';
		}

		if (step1 > (int) inShell->mStepActions.size ())
		{
			inShell->mStepActions.resize (step1);
		}

		inShell->mStepActions [step1 - 1] = commandLine;
	}

	return 0;
}

int
MRPShell::SetType (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need type (note,polypressure,control,program,pressure,pitchbend)\n");
		return 0;
	}

	unsigned char	commandByte (0);
	string	type (inShell->mParameters [1]);

	transform (type.begin (), type.end(), type.begin (), tolower);

	if (type == "note")
	{
		commandByte = MIDICLClient::kMIDINoteOnMessage;
	}
	else if (type == "polypressure")
	{
		commandByte = MIDICLClient::kMIDIPolyAftertouchMessage;
	}
	else if (type == "control")
	{
		commandByte = MIDICLClient::kMIDIControlChangeMessage;
	}
	else if (type == "program")
	{
		commandByte = MIDICLClient::kMIDIProgramChangeMessage;
	}
	else if (type == "pressure")
	{
		commandByte = MIDICLClient::kMIDIChannelAftertouchMessage;
	}
	else if (type == "pitchbend")
	{
		commandByte = MIDICLClient::kMIDIPitchBendMessage;
	}

	if (commandByte == 0)
	{
		inShell->mInterface->Write ("need type (note,polypressure,control,program,pressure,pitchbend)\n");
	}
	else
	{
		MRPSelectionSetType	typer (commandByte);

		int	selections = inShell->mSequencer->RunSelection (&typer);

		inShell->mInterface->WriteFormattedSD
			("set type %s on %d selections\n", inShell->mParameters [1].c_str (), selections);
	}

	return 0;
}

int
MRPShell::SetVelocity (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need velocity param\n");
		return 0;
	}

	int	velocity = atoi (inShell->mParameters [1].c_str ());

	if (velocity < 1 || velocity > 127)
	{
		inShell->mInterface->Write ("need velocity 1-127\n");
	}
	else
	{
		MRPSelectionSetVelocity	velociraptor (velocity);

		int	selections = inShell->mSequencer->RunSelection (&velociraptor);

		inShell->mInterface->WriteFormattedDD ("set velocity %d on %d selections\n", velocity, selections);
	}

	return 0;
}

int
MRPShell::Shell (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need shell command\n");
		return 0;
	}

	// accumulate the command line
	string	commandLine;

	vector <string>::iterator	parameter
		(inShell->mParameters.begin ());

	// skip the "shell" command
	parameter++;

	// don't quote the first parameter, it's the actual command
	commandLine += *parameter;
	commandLine += ' ';

	parameter++;

	for (; parameter != inShell->mParameters.end (); parameter++)
	{
		commandLine += '\'';
		commandLine += *parameter;
		commandLine += '\'';
		commandLine += ' ';
	}

	FILE	*results = popen (commandLine.c_str (), "r");

	if (results == NULL)
	{
		inShell->mInterface->Write ("could not spawn command\n");
	}
	else
	{
		char	buffer [1024];

		while (fgets (buffer, 1024, results) != NULL)
		{
			inShell->mInterface->Write (buffer);
		}

		fclose (results);
	}

	return 0;
}

int
MRPShell::Stop (MRPShell *inShell)
{
	inShell->mSequencer->Stop ();
	inShell->mSequencer->Reset ();

	return 0;
}

int
MRPShell::UnMute (MRPShell *inShell)
{
	MRPSelectionUnMute	unmuter;

	int	selections = inShell->mSequencer->RunSelection (&unmuter);

	inShell->mInterface->WriteFormattedD ("unmuted %d selections\n", selections);

	return 0;
}

int
MRPShell::UnMuteTrack (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need track number\n");
		return 0;
	}

	int	track = inShell->GetTrackNumber (inShell->mParameters [1]);

	if (track < 1 || track > inShell->mSequencer->GetTrackCount ())
	{
		inShell->mInterface->Write ("bad track number\n");
		return 0;
	}

	inShell->mSequencer->UnMuteTrack (track);

	inShell->mInterface->WriteFormattedD ("unmuted track %d\n", track);

	return 0;
}

int
MRPShell::UnSelectEvent (MRPShell *inShell)
{
	int	track = inShell->GetTrackNumber (inShell->mParameters [1]);
	int	step = atoi (inShell->mParameters [2].c_str ());

	if (track < 1 || track > inShell->mSequencer->GetTrackCount ())
	{
		inShell->mInterface->Write ("bad track number\n");
	}
	else
	if (step < 1 || step > inShell->mSequencer->GetStepCount ())
	{
		inShell->mInterface->Write ("bad step number\n");
	}
	else
	{
		inShell->mSequencer->UnSelectEvent (track, step);

		inShell->mInterface->WriteFormattedDD ("unselected track %d step %d\n", track, step);
	}

	return 0;
}

int
MRPShell::UnSelectStep (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need track number\n");
		return 0;
	}

	int	step = atoi (inShell->mParameters [1].c_str ());

	if (step < 1 || step > inShell->mSequencer->GetStepCount ())
	{
		inShell->mInterface->Write ("bad step number\n");
	}
	else
	{
		inShell->mSequencer->UnSelectStep (step);

		inShell->mInterface->WriteFormattedD ("unselected step %d\n", step);
	}

	return 0;
}

int
MRPShell::UnSelectTrack (MRPShell *inShell)
{
	if (inShell->mParameters.size () < 2)
	{
		inShell->mInterface->Write ("need track number\n");
		return 0;
	}

	int	track = inShell->GetTrackNumber (inShell->mParameters [1]);

	if (track < 1 || track > inShell->mSequencer->GetTrackCount ())
	{
		inShell->mInterface->Write ("bad track number\n");
	}
	else
	{
		inShell->mSequencer->UnSelectTrack (track);

		inShell->mInterface->WriteFormattedD ("unselected track %d\n", track);
	}

	return 0;
}

int
MRPShell::Vary (MRPShell *inShell)
{
	int	selections = inShell->mSequencer->VarySelection ();

	inShell->mInterface->WriteFormattedD ("varied %d selections\n", selections);

	return 0;
}

int
MRPShell::Vary2 (MRPShell *inShell)
{
	int	selections = inShell->mSequencer->VarySelection2 ();

	inShell->mInterface->WriteFormattedD ("varied %d selections\n", selections);

	return 0;
}

int
MRPShell::Version (MRPShell *inShell)
{
	inShell->mInterface->WriteFormattedSS ("MRPShell built %s %s\n", __DATE__, __TIME__);

	return 0;
}

int
MRPShell::VSolo (MRPShell *inShell)
{
	MRPSelectionVSolo	vsolo;

	int	selections = inShell->mSequencer->RunSelection (&vsolo);

	inShell->mInterface->WriteFormattedD ("v-soloed %d selections\n", selections);

	return 0;
}

int
MRPShell::VUnSolo (MRPShell *inShell)
{
	MRPSelectionVUnSolo	vunsolo;

	int	selections = inShell->mSequencer->RunSelection (&vunsolo);

	inShell->mInterface->WriteFormattedD ("v-unsoloed %d selections\n", selections);

	return 0;
}

