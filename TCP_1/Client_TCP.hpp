#include "stdafx.h"
#include "Client_TCP_interface.hpp"

template<int AddressFamily, int SocketType, int ProtocolType>
Client_TCP<AddressFamily, SocketType, ProtocolType>::Client_TCP(const std::string& hostName, const std::string& portNumber):host_name_{hostName},port_number_{portNumber}
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData_);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		throw 1;//throw real exception here
	}
	init_hints_();
	translate_ANSI_to_address_();
	init_socket_();
	connect_socket_();
}


template<int AddressFamily, int SocketType, int ProtocolType>
Client_TCP<AddressFamily, SocketType, ProtocolType>::~Client_TCP()
{//Do necessary clean up!
	perform_clean_up_();
}

/*public interface*/
template<int AddressFamily, int SocketType, int ProtocolType>
void Client_TCP<AddressFamily, SocketType, ProtocolType>::send_data()
{
#define DEFAULT_BUFLEN 512

	

	char *sendbuf = "this is a test";
	



	// Send an initial buffer
	auto iResult = send(socket_, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		perform_clean_up_();
		throw 1;//todo exceptions
	}

	printf("Bytes Sent: %ld\n", iResult);
}

template<int AddressFamily, int SocketType, int ProtocolType>
void Client_TCP<AddressFamily, SocketType, ProtocolType>::receive_data()
{
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	// Receive data until the server closes the connection
	do {
		auto iResult = recv(socket_, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);
}

/*end of public interface*/

/*private interface*/
template<int AddressFamily, int SocketType, int ProtocolType>
void Client_TCP<AddressFamily, SocketType, ProtocolType>::init_hints_()
{
	hints_.ai_family = AddressFamily;
	hints_.ai_socktype = SocketType;
	hints_.ai_protocol = ProtocolType;
}

template<int AddressFamily, int SocketType, int ProtocolType>
void Client_TCP<AddressFamily, SocketType, ProtocolType>::translate_ANSI_to_address_()
{
	auto iResult = getaddrinfo(host_name_.c_str(), port_number_.c_str(), &hints_, &result_);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		throw 1;//throw real exception here
	}
}


template<int AddressFamily, int SocketType, int ProtocolType>
void Client_TCP<AddressFamily, SocketType, ProtocolType>::init_socket_()
{
 	socket_ = socket(result_->ai_family,
			 		 result_->ai_socktype,
					 result_->ai_protocol);

	if (socket_ == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result_);
		WSACleanup();
		throw 1;
	}
}

template<int AddressFamily, int SocketType, int ProtocolType>
void Client_TCP<AddressFamily, SocketType, ProtocolType>::connect_socket_()
{
	// Connect to server.
	auto iResult = connect(socket_, result_->ai_addr, static_cast<int>(result_->ai_addrlen));
	if (iResult == SOCKET_ERROR)
	{
		closesocket(socket_);
		socket_ = INVALID_SOCKET;
		printf("Unable to connect to server!\n");
		WSACleanup();
		throw 1;//throw real exception here
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result_);

	
}

template<int AddressFamily, int SocketType, int ProtocolType>
void Client_TCP<AddressFamily, SocketType, ProtocolType>::shut_down_socket_()
{
	auto iResult{ shutdown(socket_, SD_BOTH) };
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		perform_clean_up_();
		throw 1;//todo real exception here
	}
}

template<int AddressFamily, int SocketType, int ProtocolType>
void Client_TCP<AddressFamily, SocketType, ProtocolType>::perform_clean_up_()
{
	closesocket(socket_);
	WSACleanup();
}
/*end of private interface*/