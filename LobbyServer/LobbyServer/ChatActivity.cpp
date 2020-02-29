#include "ChatActivity.h"

void ChatActivity::AddNew(int chatID, std::string chatName)
{
	if (chatLogs.find(chatID) == chatLogs.end())
	{
		chatBoxName.insert(std::pair<int, std::string>(chatID, chatName));
		currentlyChatting.insert(std::pair<int, bool>(chatID, false));
		requestsSent.insert(std::pair<int, bool>(chatID, false));
		requestsReceived.insert(std::pair<int, bool>(chatID, false));
		chatBoxBuf.insert(std::pair<int, char*>(chatID, new char[BUF_LEN]));

		memset(chatBoxBuf.at(chatID), 0, BUF_LEN);

		std::vector<std::string> newLog;
		chatLogs.insert(std::pair<int, std::vector<std::string>>(chatID, newLog));
	}
}

void ChatActivity::Remove(int chatID)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		memset(chatBoxBuf.at(chatID), 0, BUF_LEN);
		chatBoxBuf.erase(chatID);
		chatBoxName.erase(chatID);
		currentlyChatting.erase(chatID);
		requestsSent.erase(chatID);
		requestsReceived.erase(chatID);

		chatLogs.erase(chatID);
	}
}

void ChatActivity::SendRequest(int chatID)
{
	//If you can't find the chat id in our list
	if (chatLogs.find(chatID) == chatLogs.end())
	{
		return;
	}
	requestsSent.at(chatID) = true;
}

void ChatActivity::ReceiveRequest(int chatID)
{
	if (chatLogs.find(chatID) == chatLogs.end())
	{
		return;
	}
	requestsReceived.at(chatID) = true;
}

void ChatActivity::RespondRequest(int chatID, bool response)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		if (response)
		{
			currentlyChatting.at(chatID) = true;
			requestsReceived.at(chatID) = false;
			requestsSent.at(chatID) = false;
		}
		else
		{
			requestsReceived.at(chatID) = false;
			requestsSent.at(chatID) = false;
		}
	}
	else
	{
		printf("You messed up again...");
	}
}

void ChatActivity::ReceiveResponse(int chatID, bool response)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		if (response)
		{
			currentlyChatting.at(chatID) = true;
			requestsReceived.at(chatID) = false;
			requestsSent.at(chatID) = false;
		}
		else
		{
			requestsReceived.at(chatID) = false;
			requestsSent.at(chatID) = false;
		}
	}
	else
	{
		printf("You messed up again...");
	}
}

void ChatActivity::AddMessage(int chatID, std::string message)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		chatLogs.at(chatID).push_back(message);
	}
	else
	{
		printf("Shouldn't print this");
	}
}

void ChatActivity::ReceiveMessage(int chatID, std::string message)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		//Add message to log
		chatLogs.at(chatID).push_back(message);
	}
	else
	{
		printf("You messed up, this shouldn't be here.");
	}
}

bool ChatActivity::GetRequestSent(int chatID)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		//return bool
		return requestsSent.at(chatID);
	}
	else
	{
		return false;
	}
}

bool ChatActivity::GetRequestReceived(int chatID)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		//return bool
		return requestsReceived.at(chatID);
	}
	else
	{
		return false;
	}
}

bool ChatActivity::GetCurrentlyChatting(int chatID)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		//return bool
		return currentlyChatting.at(chatID);
	}
	else
	{
		return false;
	}
}

std::vector<std::string> ChatActivity::GetChatLog(int chatID)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		//return chat
		return chatLogs.at(chatID);
	}
	else
	{
		return std::vector<std::string>();
	}
}

std::string ChatActivity::GetChatName(int chatID)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		//return chat name
		return chatBoxName.at(chatID);
	}
	else
	{
		return "";
	}
}

char* ChatActivity::GetChatBufRef(int chatID)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		//return chat name
		return chatBoxBuf.at(chatID);
	}
	else
	{
		return nullptr;
	}
}

void ChatActivity::EndChat(int chatID)
{
	if (!(chatLogs.find(chatID) == chatLogs.end()))
	{
		//Add message to log
		currentlyChatting.at(chatID) = false;
		requestsSent.at(chatID) = false;
		requestsReceived.at(chatID) = false;
	}
}
