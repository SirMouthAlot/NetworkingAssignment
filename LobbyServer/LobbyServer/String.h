#pragma once
#include <string>
#include "Convertable.h"

struct String : public Convertable
{
public:
	String() : m_string("") { }
	String(std::string string) : m_string(string) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	std::string m_string;
}; 
