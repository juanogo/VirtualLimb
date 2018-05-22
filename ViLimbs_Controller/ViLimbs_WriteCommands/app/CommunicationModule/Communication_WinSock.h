//COMMUNICATION_WINSOCK_H

#ifndef COMMUNICATION_WINSOCK_H
#define COMMUNICATION_WINSOCK_H

#define  WIN                // WIN for Winsock and BSD for BSD sockets

////----- Include files ---------------------------------------------------------
#include <iostream>
#include <stdio.h>          // Needed for printf()
#include <string.h>         // Needed for memcpy() and strcpy()
#include <stdlib.h>         // Needed for exit()

#ifdef WIN
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "user32.lib")
#include <windows.h>      // Needed for all Winsock stuff
#endif
#ifdef BSD
#include <sys/types.h>    // Needed for sockets stuff
#include <netinet/in.h>   // Needed for sockets stuff
#include <sys/socket.h>   // Needed for sockets stuff
#include <arpa/inet.h>    // Needed for sockets stuff
#include <fcntl.h>        // Needed for sockets stuff
#include <netdb.h>        // Needed for sockets stuff
#endif

//----- Defines ---------------------------------------------------------------
#define  PORT_NUM         8080   // Port number used at the server


class CommunicationWinSock 
{
	public:
	CommunicationWinSock();
	~CommunicationWinSock();

	bool Initialize();
	bool Connect(char* ip,int port=8080);
	bool Send(char* command);
	bool Close();

	private:
	//////// COMMUNICATION MODULE
	//===== Main program ==========================================================
	#ifdef WIN
		WORD wVersionRequested;       // Stuff for WSA functions
		WSADATA wsaData;                              // Stuff for WSA functions
	#endif
		int                  client_s;        // Client socket descriptor
		struct sockaddr_in   server_addr;     // Server Internet address
		char		         out_buf[512];		// Output buffer for data
		int                  retcode;         // Return code

		int port;
		char* ip_address;

};



#endif //COMMUNICATION_WINSOCK_H