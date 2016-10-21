// MRPSelectionFitToKey.h

#ifndef MRPSelectionFitToKey_h
#define MRPSelectionFitToKey_h

#include "MRPSelectionAction.h"

// MRPSELECTIONFITTOKEY

class MRPSelectionFitToKey
	:
	public MRPSelectionAction
{
	// public static constants
	public:

		typedef enum
		{
			kCMajor = 0,
			kCSharpMajor,
			kDFlatMajor,
			kDMajor,
			kDSharpMajor,
			kEFlatMajor,
			kEMajor,
			kFMajor,
			kFSharpMajor,
			kGFlatMajor,
			kGMajor,
			kGSharpMajor,
			kAFlatMajor,
			kAMajor,
			kASharpMajor,
			kBFlatMajor,
			kBMajor,
			kCFlatMajor
		} Key;

		typedef enum
		{
			kBDown = -1,
			kC = 0,
			kCSharp = 1,
			kDFlat = 1,
			kD = 2,
			kDSharp = 3,
			kEFlat = 3,
			kE = 4,
			kF = 5,
			kFSharp = 6,
			kGFlat = 6,
			kG = 7,
			kGSharp = 8,
			kAFlat = 8,
			kA = 9,
			kASharp = 10,
			kBFlat = 10,
			kB = 11,
			kCUp = 12
		} Note;

	// public static methods
	public:

		static const char *
		GetKeyName (Key inKey);

		static int
		GetKeyNameCount ();

	// public constructor
	public:
		
		MRPSelectionFitToKey (Key inKey);

	// MRPSelectionAction interface
	public:

		void
		Action (MRPSequencer *inSequencer, int inTrack1, int inStep1);

	// private static methods
	private:

		static void
		InitialiseTables ();

	// private static data
	private:

		static const char *
		sKeyNames [20];

		static bool
		sInitialisedTables;

		static Note
		sKeys [20][12];
		
	// private data
	private:

		Key
		mKey;
};

#endif // MRPSelectionFitToKey_h


