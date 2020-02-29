#include "ChatData.h"

std::string ChatMessage::ToString()
{
	std::string temp;

	temp += std::to_string(m_chatID);
	temp += " ";
	temp += m_message;
	temp += "\n";

	return temp;
}

void ChatMessage::SetValue(std::string toConv)
{
	char temp[256];
	memset(temp, 0, 256);

	sscanf_s(toConv.c_str(), "%i %[^\n]s", &m_chatID, temp, 256);

	m_message = std::string(temp);
}
