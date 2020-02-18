#include "Number.h"

std::string Double::ToString()
{
	return std::to_string(m_double);
}

void Double::SetValue(std::string toConv)
{
	m_double = std::atof(toConv.c_str());
}

std::string Float::ToString()
{
	return std::to_string(m_float);
}

void Float::SetValue(std::string toConv)
{
	m_float = std::atof(toConv.c_str());
}

std::string Short::ToString()
{
	return std::to_string(m_short);
}

void Short::SetValue(std::string toConv)
{
	m_short = std::atoi(toConv.c_str());
}

std::string Long::ToString()
{
	return std::to_string(m_long);
}

void Long::SetValue(std::string toConv)
{
	m_long = std::atol(toConv.c_str());
}

std::string Int::ToString()
{
	return std::to_string(m_int);
}

void Int::SetValue(std::string toConv)
{
	m_int = std::atoi(toConv.c_str());
}

std::string Bool::ToString()
{
	return std::to_string(m_bool);
}

void Bool::SetValue(std::string toConv)
{
	m_bool = (bool)std::atoi(toConv.c_str());
}
