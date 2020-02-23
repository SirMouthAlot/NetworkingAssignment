#include "ChatData.h"

std::string ChatResponse::ToString()
{
	std::string temp;

	temp += std::to_string(m_requesterID);
	temp += " ";
	temp += std::to_string((int)m_response);

	return temp;
}

void ChatResponse::SetValue(std::string toConv)
{
	int temp;
	sscanf_s(toConv.c_str(), "%i %i", &m_requesterID, &temp);

	m_response = (bool)temp;
}

std::string ChatRequest::ToString()
{
	std::string temp;
	temp += std::to_string(m_requesterID);
	temp += " ";
	temp += std::to_string(m_chatID);

	return temp;
}

void ChatRequest::SetValue(std::string toConv)
{
	sscanf_s(toConv.c_str(), "%i %i", &m_requesterID, &m_chatID);
}

std::string ChatMessage::ToString()
{
	std::string temp;

	temp += m_message;
	temp += "\n";

	return temp;
}

void ChatMessage::SetValue(std::string toConv)
{
	char temp[256];
	memset(temp, 0, 256);

	sscanf_s(toConv.c_str(), "%[^\n]s", temp, 256);

	m_message = std::string(temp);
}
