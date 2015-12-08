#include "stdafx.h"
#include "Client_TCP.hpp"


Client_TCP::Client_TCP(const std::string& hostName,int portNumber)
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData_);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		throw 1;//throw real exception here
	}
	auto h{ hints_ };
}


Client_TCP::~Client_TCP() 
{//Do necessary clean up!

}


