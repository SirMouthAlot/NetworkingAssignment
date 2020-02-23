#include "ClientData.h"

std::string ClientData::ToString()
{
	std::string temp;

	temp += std::to_string(m_userNum);
	temp += " ";
	temp += m_userName;

	return temp;
}

void ClientData::SetValue(std::string toConv)
{
	char temp[90];
	memset(temp, 0, 90);

	sscanf_s(toConv.c_str(), "%i %s", &m_userNum, &temp, 90);

	m_userName = std::string(temp);
}

std::string ActivityData::ToString()
{
	std::string temp;
	
	temp += std::to_string(m_userNum);
	temp += " ";
	temp += std::to_string((int)m_activity);

	return temp;
}

void ActivityData::SetValue(std::string toConv)
{
	int temp;

	sscanf_s(toConv.c_str(), "%i %i", &m_userNum, &temp);

	m_activity = (Activity)temp;
}
