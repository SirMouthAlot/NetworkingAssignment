#pragma once

#include <string>

//A class that is convertable
//*Currently only converts to string but can be made to convert to other things
class Convertable
{
public:
	virtual std::string ToString()=0;
	virtual void SetValue(std::string toConv) = 0;
};