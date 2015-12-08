#include "stdafx.h"
#include "Client_TCP.hpp"


Client_TCP::Client_TCP()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData_);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		throw 1;//throw real exception here
	}
}


Client_TCP::~Client_TCP()
{
}
