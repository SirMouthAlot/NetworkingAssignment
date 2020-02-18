#include "Vector.h"
#include <string>

Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3::Vector3(float _x, float _y)
{
	x = _x;
	y = _y;
	z = 0.f;
}

Vector3::Vector3(float _x)
{
	x = _x;
	y = 0.f;
	z = 0.f;
}

Vector3::Vector3()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

std::string Vector3::ToString()
{
	std::string temp = "";
	std::string sX = std::to_string(x);
	std::string sY = std::to_string(y);
	std::string sZ = std::to_string(z);

	temp = sX;
	temp += " ";
	temp += sY;
	temp += " ";
	temp += sZ;

	return temp;
}

void Vector3::SetValue(std::string toConv)
{
	//Gets the vector out of string form
	sscanf_s(toConv.c_str(), "%f %f %f", &x, &y, &z);
}

std::ostream& operator<<(std::ostream& out, const Vector3& vec)
{
	out << "Vector3: ";
	out << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )" << std::endl;

	return out;
}