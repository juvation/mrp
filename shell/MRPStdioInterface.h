// MRPStdioInterface.h

#ifndef MRPStdioInterface_h
#define MRPStdioInterface_h

// INCLUDES

#include <stdio.h>

#include "MRPShellInterface.h"

// MRPSTDIOINTERFACE

class MRPStdioInterface
	:
	public MRPShellInterface
{
	// public constructor
	public:

		MRPStdioInterface (FILE *inStream, FILE *outStream);

	// public methods
	public:

		int
		ReadLine (char *inBuffer, int inBufferLength);

		int
		Write (const char *inBuffer);

		int
		WriteFormattedD (const char *inFormat, int inDecimal);

		int
		WriteFormattedDD (const char *inFormat, int inDecimal1, int inDecimal2);

		int
		WriteFormattedLD (const char *inFormat, long inDecimal);

		int
		WriteFormattedLF (const char *inFormat, double inFloat);

		int
		WriteFormattedLU (const char *inFormat, unsigned long inDecimal);

		int
		WriteFormattedLUD (const char *inFormat, unsigned long inDecimal1, int inDecimal2);

		int
		WriteFormattedS (const char *inFormat, const char *inString);

		int
		WriteFormattedSS (const char *inFormat, const char *inString1, const char *inString2);

		int
		WriteFormattedSD (const char *inFormat, const char *inString, int inDecimal);

	// private data members
	private:

		FILE *
		mInStream;

		FILE *
		mOutStream;
};

#endif	// MRPShellInterface_h

