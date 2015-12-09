#include "stdafx.h"
#include "Server_TCP_interface.hpp"

template<int AddressFamily, int SocketType, int Protocol, int Flags>
Server_TCP<AddressFamily, SocketType, Protocol, Flags>::Server_TCP(const std::string& portNumber):port_number_{portNumber}
{
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData_);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		throw 1;//todo real exception
	}
	make_socket_reusable_(listener_socket_);
	init_hints_();
	translate_ANSI_to_address_();
	init_listener_socket_();
}


template<int AddressFamily, int SocketType, int Protocol, int Flags>
Server_TCP<AddressFamily, SocketType, Protocol, Flags>::~Server_TCP()
{//do proper clean up

}


/*private interface*/
template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::init_hints_()
{
	hints_.ai_family = AF_INET;
	hints_.ai_socktype = SOCK_STREAM;
	hints_.ai_protocol = IPPROTO_TCP;
	hints_.ai_flags = AI_PASSIVE;
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::make_socket_reusable_(SOCKET& listener)
{
	char yes = 1;
	//char yes='1'; // Solaris people use this

	// lose the pesky "Address already in use" error message
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		printf("setsockopt");
	}
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::translate_ANSI_to_address_()
{
	int iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints_, &result_);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
}


template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::init_listener_socket_()
{
	listener_socket_ = socket(result_->ai_family, result_->ai_socktype, result_->ai_protocol);
	if (listener_socket_ == INVALID_SOCKET)
	{
		printf("Error at socket(): %d\n", WSAGetLastError());
		freeaddrinfo(result_);
		WSACleanup();
		throw 1;//todo real exceptions
	}
}
/*end of private interface*/