#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
#include <string>

template<int AddressFamily, int SocketType,int Protocol, int Flags>
class Server_TCP
{
private:
		WSADATA wsaData_;
		addrinfo *result_{ nullptr },
				 *ptr_  { nullptr };
		addrinfo  hints_{0};
		const std::string port_number_;
		SOCKET listener_socket_{ INVALID_SOCKET };
		SOCKET client_socket_{ INVALID_SOCKET };
		int received_data_length_on_that_socket_;

		void make_socket_reusable_(SOCKET& listener);
		void init_hints_();
		void translate_ANSI_to_address_();
		void init_listener_socket_();
		void bind_socket_();
		void put_socket_in_listening_state_();
		void accept_conection_();
		void receive_from_client_();
		void send_to_client_();
		void perform_cleanup_();
public:
	Server_TCP(const std::string& portNumber);
	virtual ~Server_TCP();
};

