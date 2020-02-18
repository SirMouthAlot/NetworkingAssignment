#include "Server.h"
#include <thread>
#include "Timer.h"
#include <Windows.h>

Server myServer;
bool hasFocus = false;

bool ExitProgram()
{
	while (true)
	{
		if (hasFocus)
		{
			if (GetAsyncKeyState(VK_ESCAPE))
			{
				myServer.CloseServer();

				exit(0);
			}
		}
	}
}

void UpdateFocus()
{
	while (true)
	{
		HWND thisWindow = GetConsoleWindow();
		HWND focusedWindow = GetForegroundWindow();

		if (thisWindow == focusedWindow)
		{
			hasFocus = true;
		}
		else
		{
			hasFocus = false;
		}
	}
}


void ReceiveMessages()
{
	while (true)
	{
		myServer.RecvMsg();
	}
}

int main()
{
	//Server stuffs
	myServer.InitServer();

	//Start a separate thread for exiting program
	std::thread t1, t2, t3;
	t1 = std::thread(ExitProgram);
	t2 = std::thread(ReceiveMessages);
	t3 = std::thread(UpdateFocus);
	t1.detach();
	t2.detach();
	t3.detach();

	while (true)
	{
		myServer.PrintConnectedUsers();
		system("cls");
	}

	return 0;
}