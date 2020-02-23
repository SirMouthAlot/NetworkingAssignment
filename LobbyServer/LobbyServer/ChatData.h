#ifndef __CHATDATA_H__
#define __CHATDATA_H__

#include "Convertable.h"

class ChatResponse : public Convertable
{
public:
	ChatResponse() : m_requesterID(-1), m_response(false) { }
	ChatResponse(int requesterID, bool response) : m_requesterID(requesterID), m_response(response) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	int m_requesterID;
	bool m_response;
};

class ChatRequest : public Convertable
{
public:
	ChatRequest() : m_requesterID(-1), m_chatID(-1) { }
	ChatRequest(int requestID, int chatID) : m_requesterID(requestID), m_chatID(chatID) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	int m_requesterID;
	int m_chatID;
};

class ChatMessage : public Convertable
{
public:
	ChatMessage() : m_chatID(-1), m_message("") { }
	ChatMessage(int id, std::string message) : m_chatID(id), m_message(message) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	int m_chatID;
	std::string m_message;
};

#endif // !__CHATDATA_H__
