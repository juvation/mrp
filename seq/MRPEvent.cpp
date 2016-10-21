#include "MRPEvent.h"

using namespace std;

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <sstream>

const char *MRPEvent::sNoteNames [] =
{
	"c",
	"c#",
	"d",
	"d#",
	"e",
	"f",
	"f#",
	"g",
	"g#",
	"a",
	"a#",
	"b",
};

// STATIC PUBLIC METHODS

string
MRPEvent::FormatNoteNumber (unsigned char inNote)
{
	int	octave = (inNote / 12) - 1;
	int	note = inNote % 12;

	ostringstream	noteName;

	noteName << sNoteNames [note];
	noteName << octave;

	return noteName.str ();
}

int
MRPEvent::ParseNoteNumber (string inNoteName)
{
	const char	*noteName = inNoteName.c_str ();
	int	matchedLength = 0;
	int	noteNumber = 0;

	for (int i = 0; i < 12; i++)
	{
		const char	*note (sNoteNames [i]);
		int	length (strlen (note));

		if (strncasecmp (note, noteName, length) == 0)
		{
			if (length > matchedLength)
			{
				matchedLength = length;

				const char	*octaveName (noteName + length);

				int	octave = atoi (octaveName);

				noteNumber = ((octave + 1) * 12) + i;
			}
		}
	}

	return noteNumber;
}

// PUBLIC CONSTRUCTORS

MRPEvent::MRPEvent ()
{
	mMessage = 0x90;
	mChannel = 0;
	mGateTime = 50;
	mData1 = 0;
	mData2 = 64;
	mPort = 0;
}

// PUBLIC METHODS

