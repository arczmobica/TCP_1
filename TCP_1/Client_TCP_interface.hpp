
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>


template<int AddressFamily,int SocketType, int ProtocolType>
class Client_TCP
{
private:
	WSADATA wsaData_{0};
	const std::string host_name_;
	const std::string port_number_;
	addrinfo* ptr_{nullptr};
	addrinfo* result_{nullptr};
	addrinfo hints_{0};/*If there are fewer initializers in a brace-enclosed list than there are elements or members of an aggregate, or fewer characters in a string literal used to initialize an array of known size than there are elements in the array, the remainder of the aggregate shall be initialized implicitly the same as objects that have static storage duration.*/
	SOCKET socket_{ INVALID_SOCKET };

	void init_hints_();
	void translate_ANSI_to_address_();
	void init_socket_();
	void connect_socket_();
	void shut_down_socket_();
	void perform_clean_up_();
	
public:
	Client_TCP(const std::string& hostName, const std::string& portNumber);
	virtual ~Client_TCP();
	void send_data();
	void receive_data();
};
