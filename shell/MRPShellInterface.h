// MRPShellInterface.h

#ifndef MRPShellInterface_h
#define MRPShellInterface_h

class MRPShellInterface
{
	// public constructor/destructor
	public:

		MRPShellInterface ();

		virtual ~
		MRPShellInterface ();

	// public pure virtual methods
	public:

		virtual int
		ReadLine (char *inBuffer, int inBufferLength) = 0;

		virtual int
		Write (const char *inBuffer) = 0;

		virtual int
		WriteFormattedD (const char *inFormat, int inDecimal) = 0;

		virtual int
		WriteFormattedDD (const char *inFormat, int inDecimal1, int inDecimal2) = 0;

		virtual int
		WriteFormattedLD (const char *inFormat, long inDecimal) = 0;

		virtual int
		WriteFormattedLF (const char *inFormat, double inFloat) = 0;

		virtual int
		WriteFormattedLU (const char *inFormat, unsigned long inDecimal) = 0;

		virtual int
		WriteFormattedLUD (const char *inFormat, unsigned long inDecimal1, int inDecimal2) = 0;

		virtual int
		WriteFormattedS (const char *inFormat, const char *inString) = 0;

		virtual int
		WriteFormattedSS (const char *inFormat, const char *inString1, const char *inString2) = 0;

		virtual int
		WriteFormattedSD (const char *inFormat, const char *inString1, int inDecimal) = 0;

	// public methods
	public:

		void
		SetInputDelegate (MRPShellInterface *inDelegate);

		void
		SetOutputDelegate (MRPShellInterface *inDelegate);

	// protected data members
	protected:
		
		MRPShellInterface *
		mInputDelegate;

		MRPShellInterface *
		mOutputDelegate;
};

#endif	// MRPShellInterface_h

