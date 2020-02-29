#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include "Vector.h"

class GamePosition : public Convertable
{
public:
	GamePosition() : m_chatID(-1), m_position() { }
	GamePosition(int id, Vector3 pos) : m_chatID(id), m_position(pos) { }

	virtual std::string ToString() override;
	virtual void SetValue(std::string toConv) override;

	int m_chatID;
	Vector3 m_position;
};

#endif // !__GAMEDATA_H__
