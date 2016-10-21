// MRPShellInterface.cpp

// INCLUDES

#include "MRPShellInterface.h"

#include <stdio.h>

// PUBLIC VIRTUAL CONSTRUCTOR/DESTRUCTOR

MRPShellInterface::MRPShellInterface ()
	:
	mInputDelegate (NULL),
	mOutputDelegate (NULL)
{
}

MRPShellInterface::~MRPShellInterface ()
{
}

// PUBLIC METHODS

void
MRPShellInterface::SetInputDelegate (MRPShellInterface *inDelegate)
{
	mInputDelegate = inDelegate;
}

void
MRPShellInterface::SetOutputDelegate (MRPShellInterface *inDelegate)
{
	mOutputDelegate = inDelegate;
}


