#ifndef __NETWORKINGLOBBYSCENE_H__ 
#define __NETWORKINGLOBBYSCENE_H__

#include "Scene.h"
#include "Client.h"

#include <thread>

class NetworkingLobbyScene : public Scene
{
public:
	NetworkingLobbyScene(std::string name);
	~NetworkingLobbyScene() override;

	void InitScene(float windowWidth, float windowHeight) override;

	void ReceiveMessages();

	void Update() override;
	
	void GUI() override;

private:
	Client m_client;

	std::thread m_receiveThread;

	bool m_loggedIn = false;
	int m_gamingWith = -1;
	bool m_gameRequestSent = false;
	bool m_gaming = false;
};

#endif // !__NETWORKINGLOBBYSCENE_H__ 