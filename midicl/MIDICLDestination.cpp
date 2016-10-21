// MIDICLDestination.cpp

// INCLUDES

#include "MIDICLDestination.h"

#include "MIDICLException.h"
#include "MIDICLInputPortListener.h"

// PUBLIC CONSTRUCTORS/DESTRUCTOR

MIDICLDestination::MIDICLDestination
	(MIDIClientRef inClientRef, CFStringRef inName)
	// throws MIDICLException
	:
	mListener (NULL)
{
	OSStatus	errCode = MIDIDestinationCreate
		(inClientRef, inName, ReadProc, this, &mDestinationRef);

	if (errCode != 0)
	{
		throw MIDICLException
			(MIDICLException::kMIDIDestinationCreate, errCode);
	}
}

MIDICLDestination::~MIDICLDestination ()
{
	MIDIEndpointDispose (mDestinationRef);
}

// PUBLIC METHODS

void
MIDICLDestination::SetListener (MIDICLInputPortListener *inListener)
{
	mListener = inListener;
}

#if 0
void
MIDICLDestination::SetSource (MIDIEndpointRef inSource)
{
	// MIDIPortConnectSource (mDestinationRef, inSource, NULL);
}
#endif
	
// STATIC PRIVATE METHODS

void
MIDICLDestination::ReadProc
	(const MIDIPacketList *inList, void *inReadProcRefCon,
		void *inSrcConnectionRefCon)
{
	MIDICLDestination	*self ((MIDICLDestination *) inReadProcRefCon);

	// notify listeners
	if (self->mListener != NULL)
	{
		self->mListener->Hear (inList);
	}
}

