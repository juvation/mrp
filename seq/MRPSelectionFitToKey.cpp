// MRPSelectionFitToKey.cpp

// INCLUDES

#include "MRPSelectionFitToKey.h"

#include "MRPSequencer.h"

// STATIC INITIALISATION

const char *
MRPSelectionFitToKey::sKeyNames [20];

bool
MRPSelectionFitToKey::sInitialisedTables = false;

MRPSelectionFitToKey::Note
MRPSelectionFitToKey::sKeys [20][12];

// PUBLIC STATIC METHODS

const char *
MRPSelectionFitToKey::GetKeyName (Key inKey)
{
	if (! sInitialisedTables)
	{
		InitialiseTables ();

		sInitialisedTables = true;
	}

	return sKeyNames [inKey];
}

int
MRPSelectionFitToKey::GetKeyNameCount () 
{
	return 18;
}

// MRPSELECTIONFITTOKEY

MRPSelectionFitToKey::MRPSelectionFitToKey (Key inKey)
	:
	mKey (inKey)
{
	if (! sInitialisedTables)
	{
		InitialiseTables ();

		sInitialisedTables = true;
	}
}

void
MRPSelectionFitToKey::Action (MRPSequencer *inSequencer, int inTrack1, int inStep1)
{
	if (mKey < kCMajor || mKey > kBMajor)
	{
		return;
	}

	int	note (inSequencer->GetEvent (inTrack1, inStep1).GetData1 ());

	// save away the octave
	int	octave (note / 12);

	// convert to offset from C
	note %= 12;
	
	// grab the right note from this key
	// this will round down for sharp keys and up for flat keys
	note = sKeys [mKey][note];

	// add the octave back in
	note += (octave * 12);

	inSequencer->SetNote (inTrack1, inStep1, note);
}

// PRIVATE METHODS

void
MRPSelectionFitToKey::InitialiseTables ()
{
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			sKeys [i][j] = kC;
		}
	}

	// KEY NAMES

	sKeyNames [kCMajor] = "C";
	sKeyNames [kCSharpMajor] = "C#";
	sKeyNames [kDFlatMajor] = "Db";
	sKeyNames [kDMajor] = "D";
	sKeyNames [kDSharpMajor] = "D#";
	sKeyNames [kEFlatMajor] = "Eb";
	sKeyNames [kEMajor] = "E";
	sKeyNames [kFMajor] = "F";
	sKeyNames [kFSharpMajor] = "F#";
	sKeyNames [kGFlatMajor] = "Gb";
	sKeyNames [kGMajor] = "G";
	sKeyNames [kGSharpMajor] = "G#";
	sKeyNames [kAFlatMajor] = "Ab";
	sKeyNames [kAMajor] = "A";
	sKeyNames [kASharpMajor] = "A#";
	sKeyNames [kBFlatMajor] = "Bb";
	sKeyNames [kBMajor] = "B";
	sKeyNames [kCFlatMajor] = "Cb";

	// KEYS

	// C MAJOR

	sKeys [kCMajor][kC] = kC;
	sKeys [kCMajor][kCSharp] = kC;
	sKeys [kCMajor][kD] = kD;
	sKeys [kCMajor][kDSharp] = kD;
	sKeys [kCMajor][kE] = kE;
	sKeys [kCMajor][kF] = kF;
	sKeys [kCMajor][kFSharp] = kF;
	sKeys [kCMajor][kG] = kG;
	sKeys [kCMajor][kGSharp] = kG;
	sKeys [kCMajor][kA] = kA;
	sKeys [kCMajor][kASharp] = kA;
	sKeys [kCMajor][kB] = kB;

	// G MAJOR

	sKeys [kGMajor][kC] = kC;
	sKeys [kGMajor][kCSharp] = kC;
	sKeys [kGMajor][kD] = kD;
	sKeys [kGMajor][kDSharp] = kD;
	sKeys [kGMajor][kE] = kE;
	sKeys [kGMajor][kF] = kFSharp;
	sKeys [kGMajor][kFSharp] = kFSharp;
	sKeys [kGMajor][kG] = kG;
	sKeys [kGMajor][kGSharp] = kG;
	sKeys [kGMajor][kA] = kA;
	sKeys [kGMajor][kASharp] = kA;
	sKeys [kGMajor][kB] = kB;

	// D MAJOR

	sKeys [kDMajor][kC] = kCSharp;
	sKeys [kDMajor][kCSharp] = kCSharp;
	sKeys [kDMajor][kD] = kD;
	sKeys [kDMajor][kDSharp] = kD;
	sKeys [kDMajor][kE] = kE;
	sKeys [kDMajor][kF] = kFSharp;
	sKeys [kDMajor][kFSharp] = kFSharp;
	sKeys [kDMajor][kG] = kG;
	sKeys [kDMajor][kGSharp] = kG;
	sKeys [kDMajor][kA] = kA;
	sKeys [kDMajor][kASharp] = kA;
	sKeys [kDMajor][kB] = kB;

	// A MAJOR

	sKeys [kAMajor][kC] = kCSharp;
	sKeys [kAMajor][kCSharp] = kCSharp;
	sKeys [kAMajor][kD] = kDSharp;
	sKeys [kAMajor][kDSharp] = kDSharp;
	sKeys [kAMajor][kE] = kE;
	sKeys [kAMajor][kF] = kFSharp;
	sKeys [kAMajor][kFSharp] = kFSharp;
	sKeys [kAMajor][kG] = kGSharp;
	sKeys [kAMajor][kGSharp] = kGSharp;
	sKeys [kAMajor][kA] = kA;
	sKeys [kAMajor][kASharp] = kA;
	sKeys [kAMajor][kB] = kB;

	// E MAJOR

	sKeys [kEMajor][kC] = kCSharp;
	sKeys [kEMajor][kCSharp] = kCSharp;
	sKeys [kEMajor][kD] = kDSharp;
	sKeys [kEMajor][kDSharp] = kDSharp;
	sKeys [kEMajor][kE] = kE;
	sKeys [kEMajor][kF] = kFSharp;
	sKeys [kEMajor][kFSharp] = kFSharp;
	sKeys [kEMajor][kG] = kGSharp;
	sKeys [kEMajor][kGSharp] = kGSharp;
	sKeys [kEMajor][kA] = kA;
	sKeys [kEMajor][kASharp] = kA;
	sKeys [kEMajor][kB] = kB;

	// B MAJOR

	sKeys [kBMajor][kC] = kCSharp;
	sKeys [kBMajor][kCSharp] = kCSharp;
	sKeys [kBMajor][kD] = kDSharp;
	sKeys [kBMajor][kDSharp] = kDSharp;
	sKeys [kBMajor][kE] = kE;
	sKeys [kBMajor][kF] = kFSharp;
	sKeys [kBMajor][kFSharp] = kFSharp;
	sKeys [kBMajor][kG] = kGSharp;
	sKeys [kBMajor][kGSharp] = kGSharp;
	sKeys [kBMajor][kA] = kASharp;
	sKeys [kBMajor][kASharp] = kASharp;
	sKeys [kBMajor][kB] = kB;

	// F SHARP MAJOR

	sKeys [kFSharpMajor][kC] = kCSharp;
	sKeys [kFSharpMajor][kCSharp] = kCSharp;
	sKeys [kFSharpMajor][kD] = kDSharp;
	sKeys [kFSharpMajor][kDSharp] = kDSharp;
	sKeys [kFSharpMajor][kE] = kF;
	sKeys [kFSharpMajor][kF] = kFSharp;
	sKeys [kFSharpMajor][kFSharp] = kFSharp;
	sKeys [kFSharpMajor][kG] = kGSharp;
	sKeys [kFSharpMajor][kGSharp] = kGSharp;
	sKeys [kFSharpMajor][kA] = kASharp;
	sKeys [kFSharpMajor][kASharp] = kASharp;
	sKeys [kFSharpMajor][kB] = kB;

	// C SHARP MAJOR

	sKeys [kCSharpMajor][kC] = kCSharp;
	sKeys [kCSharpMajor][kCSharp] = kCSharp;
	sKeys [kCSharpMajor][kD] = kDSharp;
	sKeys [kCSharpMajor][kDSharp] = kDSharp;
	sKeys [kCSharpMajor][kE] = kF;
	sKeys [kCSharpMajor][kF] = kFSharp;
	sKeys [kCSharpMajor][kFSharp] = kFSharp;
	sKeys [kCSharpMajor][kG] = kGSharp;
	sKeys [kCSharpMajor][kGSharp] = kGSharp;
	sKeys [kCSharpMajor][kA] = kASharp;
	sKeys [kCSharpMajor][kASharp] = kASharp;
	sKeys [kCSharpMajor][kB] = kCUp;

	// G SHARP MAJOR

	sKeys [kGSharpMajor][kC] = kCSharp;
	sKeys [kGSharpMajor][kCSharp] = kCSharp;
	sKeys [kGSharpMajor][kD] = kCSharp;
	sKeys [kGSharpMajor][kDSharp] = kDSharp;
	sKeys [kGSharpMajor][kE] = kDSharp;
	sKeys [kGSharpMajor][kF] = kF;
	sKeys [kGSharpMajor][kFSharp] = kF;
	sKeys [kGSharpMajor][kG] = kG;
	sKeys [kGSharpMajor][kGSharp] = kGSharp;
	sKeys [kGSharpMajor][kA] = kASharp;
	sKeys [kGSharpMajor][kASharp] = kASharp;
	sKeys [kGSharpMajor][kB] = kCUp;

	// FLAT KEYS

	// F MAJOR

	sKeys [kFMajor][kC] = kC;
	sKeys [kFMajor][kDFlat] = kC;
	sKeys [kFMajor][kD] = kD;
	sKeys [kFMajor][kEFlat] = kD;
	sKeys [kFMajor][kE] = kE;
	sKeys [kFMajor][kF] = kF;
	sKeys [kFMajor][kGFlat] = kF;
	sKeys [kFMajor][kG] = kG;
	sKeys [kFMajor][kAFlat] = kG;
	sKeys [kFMajor][kA] = kA;
	sKeys [kFMajor][kBFlat] = kBFlat;
	sKeys [kFMajor][kB] = kBFlat;

	// B FLAT MAJOR

	sKeys [kBFlatMajor][kC] = kC;
	sKeys [kBFlatMajor][kDFlat] = kC;
	sKeys [kBFlatMajor][kD] = kD;
	sKeys [kBFlatMajor][kEFlat] = kEFlat;
	sKeys [kBFlatMajor][kE] = kEFlat;
	sKeys [kBFlatMajor][kF] = kF;
	sKeys [kBFlatMajor][kGFlat] = kF;
	sKeys [kBFlatMajor][kG] = kG;
	sKeys [kBFlatMajor][kAFlat] = kG;
	sKeys [kBFlatMajor][kA] = kA;
	sKeys [kBFlatMajor][kBFlat] = kBFlat;
	sKeys [kBFlatMajor][kB] = kBFlat;

	// E FLAT MAJOR

	sKeys [kEFlatMajor][kC] = kC;
	sKeys [kEFlatMajor][kDFlat] = kC;
	sKeys [kEFlatMajor][kD] = kD;
	sKeys [kEFlatMajor][kEFlat] = kEFlat;
	sKeys [kEFlatMajor][kE] = kEFlat;
	sKeys [kEFlatMajor][kF] = kF;
	sKeys [kEFlatMajor][kGFlat] = kF;
	sKeys [kEFlatMajor][kG] = kG;
	sKeys [kEFlatMajor][kAFlat] = kAFlat;
	sKeys [kEFlatMajor][kA] = kAFlat;
	sKeys [kEFlatMajor][kBFlat] = kBFlat;
	sKeys [kEFlatMajor][kB] = kBFlat;

	// A FLAT MAJOR

	sKeys [kAFlatMajor][kC] = kC;
	sKeys [kAFlatMajor][kDFlat] = kDFlat;
	sKeys [kAFlatMajor][kD] = kDFlat;
	sKeys [kAFlatMajor][kEFlat] = kEFlat;
	sKeys [kAFlatMajor][kE] = kEFlat;
	sKeys [kAFlatMajor][kF] = kF;
	sKeys [kAFlatMajor][kGFlat] = kF;
	sKeys [kAFlatMajor][kG] = kG;
	sKeys [kAFlatMajor][kAFlat] = kAFlat;
	sKeys [kAFlatMajor][kA] = kAFlat;
	sKeys [kAFlatMajor][kBFlat] = kBFlat;
	sKeys [kAFlatMajor][kB] = kBFlat;

	// D FLAT MAJOR

	sKeys [kDFlatMajor][kC] = kC;
	sKeys [kDFlatMajor][kDFlat] = kDFlat;
	sKeys [kDFlatMajor][kD] = kDFlat;
	sKeys [kDFlatMajor][kEFlat] = kEFlat;
	sKeys [kDFlatMajor][kE] = kEFlat;
	sKeys [kDFlatMajor][kF] = kF;
	sKeys [kDFlatMajor][kGFlat] = kGFlat;
	sKeys [kDFlatMajor][kG] = kGFlat;
	sKeys [kDFlatMajor][kAFlat] = kAFlat;
	sKeys [kDFlatMajor][kA] = kAFlat;
	sKeys [kDFlatMajor][kBFlat] = kBFlat;
	sKeys [kDFlatMajor][kB] = kBFlat;

	// G FLAT MAJOR

	sKeys [kGFlatMajor][kC] = kC;
	sKeys [kGFlatMajor][kDFlat] = kDFlat;
	sKeys [kGFlatMajor][kD] = kDFlat;
	sKeys [kGFlatMajor][kEFlat] = kEFlat;
	sKeys [kGFlatMajor][kE] = kEFlat;
	sKeys [kGFlatMajor][kF] = kF;
	sKeys [kGFlatMajor][kGFlat] = kGFlat;
	sKeys [kGFlatMajor][kG] = kGFlat;
	sKeys [kGFlatMajor][kAFlat] = kAFlat;
	sKeys [kGFlatMajor][kA] = kAFlat;
	sKeys [kGFlatMajor][kBFlat] = kBFlat;
	sKeys [kGFlatMajor][kB] = kBFlat;

	// C FLAT MAJOR

	sKeys [kCFlatMajor][kC] = kBDown;
	sKeys [kCFlatMajor][kDFlat] = kDFlat;
	sKeys [kCFlatMajor][kD] = kDFlat;
	sKeys [kCFlatMajor][kEFlat] = kEFlat;
	sKeys [kCFlatMajor][kE] = kEFlat;
	sKeys [kCFlatMajor][kF] = kE;
	sKeys [kCFlatMajor][kGFlat] = kGFlat;
	sKeys [kCFlatMajor][kG] = kGFlat;
	sKeys [kCFlatMajor][kAFlat] = kAFlat;
	sKeys [kCFlatMajor][kA] = kAFlat;
	sKeys [kCFlatMajor][kBFlat] = kBFlat;
	sKeys [kCFlatMajor][kB] = kBFlat;
}


