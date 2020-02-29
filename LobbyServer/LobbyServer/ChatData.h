#ifndef __CHATDATA_H__
#define __CHATDATA_H__

#include "Convertable.h"

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
