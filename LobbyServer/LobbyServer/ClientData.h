#ifndef __CLIENT_DATA_H__
#define __CLIENT_DATA_H__

#include "Convertable.h"

enum class Activity
{
	AVAILABLE,
	BUSY_CHATTING,
	BUSY_PLAYINGGAME
};

class ClientData : public Convertable
{
public:
	ClientData() : m_userNum(-1), m_userName("Default") { }
	ClientData(int userNum, std::string userName) : m_userNum(userNum), m_userName(userName) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	std::string m_userName;
	int m_userNum;
};

class ActivityData : public Convertable
{
public:
	ActivityData() : m_userNum(-1), m_activity(Activity::AVAILABLE) { }
	ActivityData(int userNum, Activity act) : m_userNum(userNum), m_activity(act) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	int m_userNum;
	Activity m_activity;
};



#endif // !__CLIENT_DATA_H__
