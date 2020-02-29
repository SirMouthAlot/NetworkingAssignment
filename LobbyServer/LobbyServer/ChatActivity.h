#ifndef __CHATACTIVITY_H__
#define __CHATACTIVITY_H__

#include "NetworkingWrapper.h"
#include <unordered_map>

struct ChatActivity
{
	ChatActivity()
	{ }

	void AddNew(int chatID, std::string chatName);
	void Remove(int chatID);

	void SendRequest(int chatID);
	void ReceiveRequest(int chatID);
	void RespondRequest(int chatID, bool response);
	void ReceiveResponse(int chatID, bool response);
	void AddMessage(int chatID, std::string message);
	void ReceiveMessage(int chatID, std::string message);

	bool GetRequestSent(int chatID);
	bool GetRequestReceived(int chatID);
	bool GetCurrentlyChatting(int chatID);
	std::vector<std::string> GetChatLog(int chatID);
	std::string GetChatName(int chatID);
	char* GetChatBufRef(int chatID);

	void EndChat(int chatID);

	std::unordered_map<int, char*> chatBoxBuf;
	std::unordered_map<int, std::string> chatBoxName;
	std::unordered_map<int, bool> currentlyChatting;
	std::unordered_map<int, bool> requestsSent;
	std::unordered_map<int, bool> requestsReceived;
	std::unordered_map<int, std::vector<std::string>> chatLogs;

private:
	unsigned int BUF_LEN = 512;
};


#endif // !__CHATACTIVITY_H__

