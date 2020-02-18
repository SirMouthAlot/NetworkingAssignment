#include "String.h"

std::string String::ToString()
{
	return m_string;
}

void String::SetValue(std::string toConv)
{
	m_string = toConv;
}
