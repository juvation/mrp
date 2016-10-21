// MRPStdioInterface.h

// INCLUDES

#include "MRPStdioInterface.h"

#include <string.h>

// MRPSTDIOINTERFACE

MRPStdioInterface::MRPStdioInterface (FILE *inStream, FILE *outStream)
	:
	mInStream (inStream),
	mOutStream (outStream)
{
}

int
MRPStdioInterface::ReadLine (char *inBuffer, int inBufferLength)
{
	if (mInputDelegate == NULL)
	{
		const char	*buffer = fgets (inBuffer, inBufferLength, mInStream);
		int	returnCode = -1;

		if (buffer != NULL)
		{
			returnCode = strlen (inBuffer);

			// strip the newline that fgets always preserves
			inBuffer [returnCode - 1] = 0;

			returnCode--;
		}

		return returnCode;
	}
	else
	{
		return mInputDelegate->ReadLine (inBuffer, inBufferLength);
	}
}

int
MRPStdioInterface::Write (const char *inBuffer)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		if (mOutStream != NULL)
		{
			returnCode = fprintf (mOutStream, "%s", inBuffer);

			fflush (mOutStream);
		}
	}
	else
	{
		returnCode = mOutputDelegate->Write (inBuffer);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedD (const char *inFormat, int inDecimal)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inDecimal);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedLD (inFormat, inDecimal);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedDD (const char *inFormat, int inDecimal1, int inDecimal2)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inDecimal1, inDecimal2);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedDD (inFormat, inDecimal1, inDecimal2);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedLD (const char *inFormat, long inDecimal)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inDecimal);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedLD (inFormat, inDecimal);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedLF (const char *inFormat, double inFloat)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inFloat);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedLF (inFormat, inFloat);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedLU (const char *inFormat, unsigned long inDecimal)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inDecimal);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedLU (inFormat, inDecimal);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedLUD (const char *inFormat, unsigned long inDecimal1, int inDecimal2)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inDecimal1, inDecimal2);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedLUD (inFormat, inDecimal1, inDecimal2);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedS (const char *inFormat, const char *inString)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inString);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedS (inFormat, inString);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedSS (const char *inFormat, const char *inString1, const char *inString2)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inString1, inString2);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedSS (inFormat, inString1, inString2);
	}

	return returnCode;
}

int
MRPStdioInterface::WriteFormattedSD (const char *inFormat, const char *inString, int inDecimal)
{
	int	returnCode = 1;

	if (mOutputDelegate == NULL)
	{
		returnCode = fprintf (mOutStream, inFormat, inString, inDecimal);
	}
	else
	{
		returnCode = mOutputDelegate->WriteFormattedSD (inFormat, inString, inDecimal);
	}

	return returnCode;
}

