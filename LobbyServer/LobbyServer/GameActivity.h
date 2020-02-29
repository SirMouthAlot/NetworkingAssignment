#ifndef __GAMEACTIVITY_H__
#define __GAMEACTIVITY_H__

#include <string>
#include <unordered_map>
#include "NetworkingWrapper.h"

struct GameActivity
{
	GameActivity() { }

	void AddNew(int gameID, std::string _gameName);
	void Remove(int gameID);

	void SendRequest(int gameID);
	void ReceiveRequest(int gameID);
	void RespondRequest(int gameID, bool response);
	void ReceiveResponse(int gameID, bool response);
	void ReceivePosition(int gameID, Vector3 pos);

	bool GetRequestSent(int gameID);
	bool GetRequestReceived(int gameID);
	bool GetCurrentlyGaming(int gameID);
	std::string GetGameName(int gameID);
	Vector3 GetCurrentPosition(int gameID);

	void EndGame(int gameID);

	std::unordered_map<int, std::string> gameName;
	std::unordered_map<int, bool> currentlyGaming;
	std::unordered_map<int, bool> requestsSent;
	std::unordered_map<int, bool> requestsReceived;
	std::unordered_map<int, Vector3> currentPosition;
};


#endif // !__GAMEACTIVITY_H__

