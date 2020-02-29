#include "GameActivity.h"

void GameActivity::AddNew(int gameID, std::string _gameName)
{
	if (gameName.find(gameID) == gameName.end())
	{
		gameName.insert(std::pair<int, std::string>(gameID, _gameName));
		currentlyGaming.insert(std::pair<int, bool>(gameID, false));
		requestsSent.insert(std::pair<int, bool>(gameID, false));
		requestsReceived.insert(std::pair<int, bool>(gameID, false));
		currentPosition.insert(std::pair<int, Vector3>(gameID, Vector3()));
	}
}

void GameActivity::Remove(int gameID)
{
	if (gameName.find(gameID) != gameName.end())
	{
		gameName.erase(gameID);
		currentlyGaming.erase(gameID);
		requestsSent.erase(gameID);
		requestsReceived.erase(gameID);
		currentPosition.erase(gameID);
	}
}

void GameActivity::SendRequest(int gameID)
{
	if (gameName.find(gameID) == gameName.end())
	{
		return;
	}
	requestsSent.at(gameID) = true;
}

void GameActivity::ReceiveRequest(int gameID)
{
	if (gameName.find(gameID) == gameName.end())
	{
		return;
	}
	requestsReceived.at(gameID) = true;
}

void GameActivity::RespondRequest(int gameID, bool response)
{
	if (gameName.find(gameID) != gameName.end())
	{
		if (response)
		{
			currentlyGaming.at(gameID) = true;
			requestsReceived.at(gameID) = false;
			requestsSent.at(gameID) = false;
		}
		else
		{
			requestsReceived.at(gameID) = false;
			requestsSent.at(gameID) = false;
		}
	}
	else
	{
		printf("You messed up");
	}
}

void GameActivity::ReceiveResponse(int gameID, bool response)
{
	if (gameName.find(gameID) != gameName.end())
	{
		if (response)
		{
			currentlyGaming.at(gameID) = true;
			requestsReceived.at(gameID) = false;
			requestsSent.at(gameID) = false;
		}
		else
		{
			requestsReceived.at(gameID) = false;
			requestsSent.at(gameID) = false;
		}
	}
	else
	{
		printf("You messed up AGAIN");
	}
}

void GameActivity::ReceivePosition(int gameID, Vector3 pos)
{
	if (gameName.find(gameID) != gameName.end())
	{
		currentPosition.at(gameID) = pos;
	}
	else
	{
		printf("You kinda screwed up here");
	}
}

bool GameActivity::GetRequestSent(int gameID)
{
	if (gameName.find(gameID) != gameName.end())
	{
		return requestsSent.at(gameID);
	}
	else
	{
		return false;
	}
}

bool GameActivity::GetRequestReceived(int gameID)
{
	if (gameName.find(gameID) != gameName.end())
	{
		return requestsReceived.at(gameID);
	}
	else
	{
		return false;
	}
}

bool GameActivity::GetCurrentlyGaming(int gameID)
{
	if (gameName.find(gameID) != gameName.end())
	{
		return currentlyGaming.at(gameID);
	}
	else
	{
		return false;
	}
}

std::string GameActivity::GetGameName(int gameID)
{
	if (gameName.find(gameID) != gameName.end())
	{
		return gameName.at(gameID);
	}
	else
	{
		return "";
	}
}

Vector3 GameActivity::GetCurrentPosition(int gameID)
{
	if (gameName.find(gameID) != gameName.end())
	{
		return currentPosition.at(gameID);
	}
	else
	{
		return Vector3();
	}
}

void GameActivity::EndGame(int gameID)
{
	if (gameName.find(gameID) != gameName.end())
	{
		currentlyGaming.at(gameID) = false;
		requestsSent.at(gameID) = false;
		requestsReceived.at(gameID) = false;
	}
}


