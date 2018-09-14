#pragma once

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <time.h>
#include <iostream>
#include <fstream>
#include <assert.h>

#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

namespace Simple_TCP
{
	void terminate(SOCKET _socket);

	void init_Network()
	{
		WSADATA wsadata;
		int r = WSAStartup(MAKEWORD(2, 2), &wsadata);
		assert(r == 0);
		Sleep(200);
	}

	hostent *get_Host(const char *hostname)
	{
		int n_tries = 0;
		hostent *host = NULL;
		while (host == NULL)
		{
			host = gethostbyname(hostname);
			if (++n_tries >= 10) assert(0);
			Sleep(100);
		}
		return host;
	}

	SOCKET make_Nonblocking_TCP_Connection(hostent *host, int port)
	{
		SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket == INVALID_SOCKET)
		{
			printf("socket failure\n");
			assert(0);
		}
		unsigned long mode = 1;
		int r = ioctlsocket(_socket, FIONBIO, &mode);
		if (r != 0)
		{
			printf("ioctlsocket failed: %d\n", r);
			assert(0);
		}

		SOCKADDR_IN sockaddr;
		sockaddr.sin_port = htons(port);
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

		for (;;)
		{
			int r = connect(_socket, (SOCKADDR*)(&sockaddr), sizeof(SOCKADDR_IN));
			if (r == 0)
			{
				printf("connected to: %s\n", host->h_name);
				break;
			}
			else if (r < 0)
			{
				int error_code = WSAGetLastError();
				if (error_code == WSAEISCONN)
				{
					printf("connected to: %s\n", host->h_name);
					break;
				}
				else if (error_code == WSAEWOULDBLOCK)
				{
					printf("waiting for network interface\n");
					Sleep(200);
				}
				else if (error_code == WSAEALREADY)
				{
					printf("initiating connection\n");
					Sleep(200);
				}
				else
				{
					printf("error_code: %d\n", error_code);
					terminate(_socket);
					assert(0);
				}
			}
			else
			{
				printf("could not connect to: %s\n", host->h_name);
				terminate(_socket);
				assert(0);
			}
		}

		return _socket;
	}
	
	SOCKET make_Nonblocking_TCP_Listener(int port)
	{
		SOCKET _socket = socket(AF_INET, SOCK_STREAM, 0);
		if (_socket == INVALID_SOCKET)
		{
			printf("socket failure\n");
			assert(0);
		}
		unsigned long mode = 1;
		int r = ioctlsocket(_socket, FIONBIO, &mode);
		if (r != 0)
		{
			printf("ioctlsocket failed: %d\n", r);
			assert(0);
		}

		SOCKADDR_IN sock_addr = {};
		sock_addr.sin_port = htons(port);
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		int counter = 0;
		for (;;)
		{
			r = bind(_socket, (SOCKADDR*)&sock_addr, sizeof(sock_addr));
			if (r >= 0) break;
			if (r < 0 && ++counter > 10)
			{
				printf("could not bind socket\n");
				assert(0);
			}
			Sleep(10);
		}

		r = listen(_socket, 4);
		if (r < 0)
		{
			printf("could not start listening\n");
			assert(0);
		}
		Sleep(50);
		return _socket;
	}

	SOCKET accept(SOCKET _socket)
	{
		sockaddr client_addr = {};
		int client_addr_size = sizeof(sockaddr);
		SOCKET s = accept(_socket, &client_addr, &client_addr_size);
		return s;
	}

	int read(char *buffer,int buffer_size, SOCKET _socket)
	{
		memset(buffer, 0, buffer_size);
		//read into buffer
		int numbytes = recv(_socket, buffer, buffer_size, 0);
		if (numbytes < 0)
		{
			//nothing received
			int error_code = WSAGetLastError();
			if (error_code != WSAEWOULDBLOCK)
			{
				printf("WSAGetLastError: %d\n", error_code);
			}
			return -1;
		}

		if (numbytes == 0)
		{
			printf("connection terminated\n");
			terminate(_socket);
			return 0;
		}
		buffer[numbytes] = '\0';

		return numbytes;
	}

	void close(SOCKET _socket)
	{
		shutdown(_socket, SD_SEND);
		closesocket(_socket);
	}

	void terminate(SOCKET _socket)
	{
		WSACleanup();
	}
}
