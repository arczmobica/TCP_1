#include "stdafx.h"
#include "Server_TCP_interface.hpp"
#include "TCP_Exception.h"
template<int AddressFamily, int SocketType, int Protocol, int Flags>
Server_TCP<AddressFamily, SocketType, Protocol, Flags>::Server_TCP(const std::string& portNumber):port_number_{portNumber}
{
	last_result_ = WSAStartup(MAKEWORD(2, 2), &wsaData_);
	if (last_result_ != 0)
	{
		printf("WSAStartup failed: %d\n", last_result_);
		throw CTCP_Exception();//todo real exception
	}
	
	init_hints_();
	translate_ANSI_to_address_();
	init_listener_socket_();
	bind_socket_();
	put_socket_in_listening_state_();
	accept_conection_();
}


template<int AddressFamily, int SocketType, int Protocol, int Flags>
Server_TCP<AddressFamily, SocketType, Protocol, Flags>::~Server_TCP()
{//do proper clean up

}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
int Server_TCP<AddressFamily, SocketType, Protocol, Flags>::getLastResult()const
{
	return last_result_;
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
		printf("setsockopt: %d", WSAGetLastError());
	}
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::translate_ANSI_to_address_()
{
	last_result_ = getaddrinfo(nullptr, port_number_.c_str(), &hints_, &result_);
	if (last_result_ != 0)
	{
		printf("getaddrinfo failed: %d\n", last_result_);
		WSACleanup();
		throw CTCP_Exception();//todo real exceptions
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
		throw CTCP_Exception();//todo real exceptions
	}
	make_socket_reusable_(listener_socket_);
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::bind_socket_()
{
	last_result_ = bind(listener_socket_, result_->ai_addr, (int)result_->ai_addrlen);
	if (last_result_ == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result_);
		closesocket(listener_socket_);
		WSACleanup();
		throw CTCP_Exception();//todo real exceptions
	}
	freeaddrinfo(result_);
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::put_socket_in_listening_state_()
{
	if (listen(listener_socket_, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %d\n", WSAGetLastError());
		closesocket(listener_socket_);
		WSACleanup();
		throw CTCP_Exception();//todo real exceptions
	}
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::accept_conection_()
{
	// Accept a client socket
	client_socket_ = accept(listener_socket_, nullptr, nullptr);
	if (client_socket_ == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(listener_socket_);
		WSACleanup();
		throw CTCP_Exception();//todo real exceptions
	}
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::receive_from_client()
{
	const int DEFAULT_BUFLEN{ 512 };
	
	char recvbuf[DEFAULT_BUFLEN];
	int  recvbuflen = DEFAULT_BUFLEN;

	received_data_length_on_that_socket_ = recv(client_socket_, recvbuf, recvbuflen, 0);
	if(received_data_length_on_that_socket_ == SOCKET_ERROR)
	{
		printf("recv failed: %d\n", WSAGetLastError());
		closesocket(client_socket_);
		WSACleanup();
		throw CTCP_Exception();//todo real exceptions
	}
	printf("Bytes received: %d\n", received_data_length_on_that_socket_);
	
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::send_to_client()
{
	const int DEFAULT_BUFLEN{ 512 };

	char recvbuf[DEFAULT_BUFLEN];
	// Echo the buffer back to the sender
	auto iSendResult = send(client_socket_, recvbuf, received_data_length_on_that_socket_, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(client_socket_);
		WSACleanup();
		throw CTCP_Exception();//todo real exceptions
	}
	printf("Bytes sent: %d\n", iSendResult);
}

template<int AddressFamily, int SocketType, int Protocol, int Flags>
inline void Server_TCP<AddressFamily, SocketType, Protocol, Flags>::perform_cleanup_()
{
	closesocket(client_socket_);
	WSACleanup();
}
/*end of private interface*/