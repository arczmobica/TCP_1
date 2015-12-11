#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TCP_EXCEPTION_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TCP_EXCEPTION_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TCP_EXCEPTION_EXPORTS
#define TCP_EXCEPTION_API __declspec(dllexport)
#else
#define TCP_EXCEPTION_API __declspec(dllimport)
#endif
#include <stdexcept>
#include <string>
using std::string;
// This class is exported from the TCP_Exception.dll
class TCP_EXCEPTION_API CTCP_Exception : public std::runtime_error
{
private:
	int error_no_;
public:
	CTCP_Exception(void);
	CTCP_Exception(int last_error);
	// TODO: add your methods here.
	
};

extern TCP_EXCEPTION_API int nTCP_Exception;

TCP_EXCEPTION_API int fnTCP_Exception(void);
