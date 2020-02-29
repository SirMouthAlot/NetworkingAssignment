#include "GameData.h"

std::string GamePosition::ToString()
{
	std::string temp;

	temp += std::to_string(m_chatID);
	temp += " ";
	temp += std::to_string(m_position.x);
	temp += " ";
	temp += std::to_string(m_position.y);
	temp += " ";
	temp += std::to_string(m_position.z);

	return temp;
}

void GamePosition::SetValue(std::string toConv)
{
	sscanf_s(toConv.c_str(), "%i %f %f %f", &m_chatID, &m_position.x, &m_position.y, &m_position.z);
}
