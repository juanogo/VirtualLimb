#include "Communication_WinSock.h"


CommunicationWinSock::CommunicationWinSock()
{

}

CommunicationWinSock::~CommunicationWinSock()
{
	//Close();
}

bool CommunicationWinSock::Initialize()
{
	wVersionRequested = MAKEWORD(1, 1);

	#ifdef WIN
		// This stuff initializes winsock
		WSAStartup(wVersionRequested, &wsaData);
	#endif

	// >>> Step #1 <<<
	// Create a client socket
	//   - AF_INET is Address Family Internet and SOCK_STREAM is streams
	client_s = socket(AF_INET, SOCK_STREAM, 0);
	if (client_s < 0)
	{
		printf("*** ERROR - Initialize socket() failed \n");
		return false;
		exit(-1);
	}
	return true;
}


bool CommunicationWinSock::Connect(char* ip, int port_num)
{
	// >>> Step #2 <<<
	// Fill-in the server's address information and do a connect with the
	// listening server using the client socket - the connect() will block.
	ip_address = ip;
	port = port_num;
	
	std::cout << "Connecting to " << ip_address << " Port: " << port << std::endl;

	server_addr.sin_family = AF_INET;                 // Address family to use
	server_addr.sin_port = htons(port);					// Port num to use
	server_addr.sin_addr.s_addr = inet_addr(ip_address); // IP address to use
	retcode = connect(client_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
	
	if (retcode < 0)
	{
		printf("*** ERROR - connect() failed \n");
		return false;
		exit(-1);
	}
	return true;

}

bool CommunicationWinSock::Send(char* command)
{
	// >>> Step #4 <<<
	// Send to the server using the client socket
	retcode = send(client_s, command, (strlen(command) + 1), 0);

	if (retcode < 0)
	{
		printf("*** ERROR - send() failed \n");
		return false;
		exit(-1);
	}//fi
	return true;
}

bool CommunicationWinSock::Close()
{
	// >>> Step #5 <<<
	// Close the client socket
#ifdef WIN
	retcode = closesocket(client_s);
	if (retcode < 0)
	{
		printf("*** ERROR - closesocket() failed \n");
		return false;
		exit(-1);
	}
#endif
#ifdef BSD
	retcode = close(client_s);
	if (retcode < 0)
	{
		printf("*** ERROR - close() failed \n");
		return false;
		exit(-1);
	}
#endif

#ifdef WIN
	// Clean-up winsock
	WSACleanup();
#endif
	return true;
}

	

	




	



