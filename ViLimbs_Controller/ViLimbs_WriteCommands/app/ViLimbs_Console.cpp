/*
* Application to send commands to ViLimbs using WinSock.
* Myo and EPOC devices are not required.
*/

#include "CommunicationModule\Communication_WinSock.h"

#define _USE_MATH_DEFINES
#include <array>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <algorithm>


// Read file to config events with commands in ViLimbs server.
char com1[3],com2[3],com3[3],com4[3],com5[3],com6[3];


// Main procedure
int main(int argc, char** argv)
{

	//// COMMUNICATION WITH WINSOCKET
	char *ip_connection;								// Filename for file, IPdir for connection.
	bool sendViaWinSock = false, sentOK = false;		// Determine wheter send data via LAN
	CommunicationWinSock comm;							// Communication module.

	
	/////////// Arguments configuration
	if(argc<1 && argc > 2)
	{
		std::cout << "Invalid number of inputs." << std::endl;
		return 1;
	}

	/// Execute in localhost with filename "MyoData.txt" and default thresholds
	if(argc == 1)
	{
		sendViaWinSock = false;
	}
	/// Send data to specified IP with filename "MyoData.txt"
	else if(argc == 2)
	{
		ip_connection = argv[1];
	}

	// Initialize communication module
	if(argc==2)
	{
		sendViaWinSock = true;
		comm = CommunicationWinSock();
		comm.Initialize();
		bool connectedToIP = comm.Connect(ip_connection);
		if (!connectedToIP)
		{
			std::cout << "Can not reach specified IP destination.";
			return 0;
		}
		else
		{
			std::cout << "Connected to specified destination.";
		}
	}

	///// Initialize commands list
	strcpy(com1,"RH");
	strcpy(com2,"EH");
	strcpy(com3,"FH");
	strcpy(com4,"CH");
	strcpy(com5,"FA");
	strcpy(com6,"EA");


	try{
		// Finally we enter our main loop.
		bool exitLoop = false;
		while (!exitLoop) 
		{
			//// Receive command
			std::string commandId;
			std::cout << "----Command to send ( RH|EH|FH|CH|FA|EA|exit ): ";
			std::cin >> commandId;

			if (commandId.compare("RH") == 0 || commandId.compare("rh") == 0)
			{
				std::cout << "Sending command RH." << std::endl;
				if(sendViaWinSock)
					sentOK=comm.Send(com1);
			}
			else if(commandId.compare("EH") == 0 || commandId.compare("eh") == 0)
			{
				std::cout << "Sending command EH." << std::endl;
				if(sendViaWinSock)
					sentOK=comm.Send(com2);
			}
			else if(commandId.compare("FH") == 0 || commandId.compare("fh") == 0)
			{
				std::cout << "Sending command FH." << std::endl;
				if(sendViaWinSock)
					sentOK=comm.Send(com3);
			}
			else if(commandId.compare("CH") == 0 || commandId.compare("ch") == 0)
			{
				std::cout << "Sending command CH." << std::endl;
				if(sendViaWinSock)
					sentOK=comm.Send(com4);
			}
			else if(commandId.compare("FA") == 0 || commandId.compare("fa") == 0)
			{
				std::cout << "Sending command FA." << std::endl;
				if(sendViaWinSock)
					sentOK=comm.Send(com5);
			}
			else if(commandId.compare("EA") == 0 || commandId.compare("ea") == 0)
			{
				std::cout << "Sending command EA." << std::endl;
				if(sendViaWinSock)
					sentOK=comm.Send(com6);
			}
			else if(commandId.compare("Exit") == 0 || commandId.compare("exit") == 0 || commandId.compare("EXIT") == 0)
			{
				break;
			}
			else
			{
				std::cout << "Cannot identify written command." << std::endl;
			}

			if(sendViaWinSock && !sentOK)
			{
				std::cout << "Command was not sent. Please try again." << std::endl;
			}

	}
	
	////////////CLOSE APPLICATION
	//// Detect ESC key to finish application
	//if( GetAsyncKeyState(VK_ESCAPE) & 0x8000 )
	//{
		// Close Winsock
		if(sendViaWinSock)
		{
			comm.Close();
			std::cout << "WinSocket successfully closed" << std::endl;
		}
		std::cout << "Exiting application..." << std::endl;
	//}

    // If a standard exception occurred, we print out its message and exit.
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();

		return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;

}
