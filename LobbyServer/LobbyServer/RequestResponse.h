#ifndef __REQUESTRESPONSE_H__
#define __REQUESTRESPONSE_H__

#include "Convertable.h"

class Response : public Convertable
{
public:
	Response() : m_requesterID(-1), m_response(false) { }
	Response(int requesterID, bool response) : m_requesterID(requesterID), m_response(response) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	int m_requesterID;
	bool m_response;
};

class Request : public Convertable
{
public:
	Request() : m_requesterID(-1), m_chatID(-1) { }
	Request(int requestID, int chatID) : m_requesterID(requestID), m_chatID(chatID) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	int m_requesterID;
	int m_chatID;
};


#endif // !__REQUESTRESPONSE_H__
