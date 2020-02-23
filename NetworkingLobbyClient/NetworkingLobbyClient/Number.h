#pragma once
#include <string>
#include <iostream>
#include <cstdlib>

#include "Convertable.h"

class Bool : public Convertable
{
public:
	Bool() : m_bool(false) { }
	Bool(bool bl) : m_bool(bl) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	bool m_bool;
};

struct Int : public Convertable
{
public:
	Int() : m_int(0) { }
	Int(int it) : m_int(it) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	int m_int;
};

struct Long : public Convertable
{
public:
	Long() : m_long(0) { }
	Long(long lng) : m_long(lng) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	long m_long;
};

struct Short : public Convertable
{
public:
	Short() : m_short(0) { }
	Short(short srt) : m_short(srt) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	short m_short;
};

struct Float : public Convertable
{
public:
	Float() : m_float(0.f) { }
	Float(float flt) : m_float(flt) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	float m_float;
};

struct Double : public Convertable
{
public:
	Double() : m_double(0.0) { }
	Double(double dbl) : m_double(dbl) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	double m_double;
};