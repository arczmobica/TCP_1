// TCP_Exception.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "TCP_Exception.h"


// This is an example of an exported variable
TCP_EXCEPTION_API int nTCP_Exception=0;

// This is an example of an exported function.
TCP_EXCEPTION_API int fnTCP_Exception(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see TCP_Exception.h for the class definition
CTCP_Exception::CTCP_Exception()
{
    return;
}