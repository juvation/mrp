// MIDICLInputPort.cpp

// INCLUDES

#include "MIDICLInputPort.h"

#include "MIDICLException.h"
#include "MIDICLInputPortListener.h"

// PUBLIC CONSTRUCTORS/DESTRUCTOR

MIDICLInputPort::MIDICLInputPort
	(MIDIClientRef inClientRef, CFStringRef inName)
	// throws MIDICLException
	:
	mListener (NULL)
{
	OSStatus	errCode = MIDIInputPortCreate
		(inClientRef, inName, ReadProc, this, &mPortRef);

	if (errCode != 0)
	{
		throw MIDICLException
			(MIDICLException::kMIDIInputPortCreate, errCode);
	}
}

MIDICLInputPort::~MIDICLInputPort ()
{
	MIDIPortDispose (mPortRef);
}

// PUBLIC METHODS

void
MIDICLInputPort::SetListener (MIDICLInputPortListener *inListener)
{
	mListener = inListener;
}

void
MIDICLInputPort::SetSource (MIDIEndpointRef inSource)
{
	MIDIPortConnectSource (mPortRef, inSource, NULL);
}
	
// STATIC PRIVATE METHODS

void
MIDICLInputPort::ReadProc
	(const MIDIPacketList *inList, void *inReadProcRefCon,
		void *inSrcConnectionRefCon)
{
	MIDICLInputPort	*self ((MIDICLInputPort *) inReadProcRefCon);

	// notify listeners
	if (self->mListener != NULL)
	{
		self->mListener->Hear (inList);
	}
}

