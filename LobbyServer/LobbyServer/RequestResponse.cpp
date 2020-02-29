#include "RequestResponse.h"

std::string Response::ToString()
{
	std::string temp;

	temp += std::to_string(m_requesterID);
	temp += " ";
	temp += std::to_string((int)m_response);

	return temp;
}

void Response::SetValue(std::string toConv)
{
	int temp;
	sscanf_s(toConv.c_str(), "%i %i", &m_requesterID, &temp);

	m_response = (bool)temp;
}

std::string Request::ToString()
{
	std::string temp;
	temp += std::to_string(m_requesterID);
	temp += " ";
	temp += std::to_string(m_chatID);
	return temp;
}

void Request::SetValue(std::string toConv)
{
	sscanf_s(toConv.c_str(), "%i %i", &m_requesterID, &m_chatID);
}