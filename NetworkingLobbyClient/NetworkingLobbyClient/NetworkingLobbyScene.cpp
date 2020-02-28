#include "NetworkingLobbyScene.h"
#include "UI.h"

NetworkingLobbyScene::NetworkingLobbyScene(std::string name)
{
	b2Vec2 gravity = b2Vec2(float32(0.f), float32(0.f));
	m_physicsWorld->SetGravity(gravity);
}

NetworkingLobbyScene::~NetworkingLobbyScene()
{
	if (m_loggedIn)
	{
		m_client.DisconnectFromServer();
	}
	m_client.ShutdownClient();
}

void NetworkingLobbyScene::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register (so when you unload the scene when you switch between scenes
	//you can later reInit this scene
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup Main Camera entity
	{
		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Camera");
		ECS::SetIsMainCamera(entity, true);
	}


	m_client.InitClient();

	//Sets up the receive thread
	m_receiveThread = std::thread(&NetworkingLobbyScene::ReceiveMessages, this);
	m_receiveThread.detach();
}

void NetworkingLobbyScene::ReceiveMessages()
{
	while (true)
	{
		if (m_loggedIn)
		{
			MessageType type;
			//Temp keeps returning as a nullptr :/
			Convertable* temp = m_client.RecvMsg(type);

			if (type == MessageType::MSG_CHATREQUEST)
			{
				Int tempInt = *(Int*)temp;

				int chatID = tempInt.m_int;

				m_client.m_chatActivity.ReceiveRequest(chatID);
			}
			else if (type == MessageType::MSG_CHATRESPONSE)
			{
				ChatResponse tempResponse = *(ChatResponse*)temp;

				bool response = tempResponse.m_response;
				int chatID = tempResponse.m_requesterID;

				m_client.m_chatActivity.ReceiveResponse(chatID, response);
			}
			else if (type == MessageType::MSG_CHATSTRING)
			{
				ChatMessage tempMessage = *(ChatMessage*)temp;

				int chatID = tempMessage.m_chatID;

				m_client.m_chatActivity.ReceiveMessage(chatID, tempMessage.m_message);
			}
		}
	}
}

void NetworkingLobbyScene::Update()
{

}

void NetworkingLobbyScene::GUI()
{
	ImGui::Begin("Networking Lobby");

	static bool init = false;
	static bool chatInit = false;

	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	static char buf_2[BUF_LEN];
	if (init == false)
	{
		memset(buf, 0, BUF_LEN);
		memset(buf_2, 0, BUF_LEN);
		init = true;
	}

	if (!m_loggedIn)
	{
		ImGui::InputText("Input Username Here", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);
		ImGui::InputText("Input IP here", buf_2, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

		if (ImGui::Button("Login"))
		{
			//Connects to server with IP and username
			m_client.ConnectToServer(buf_2, buf);

			m_loggedIn = true;
		}

		if (ImGui::Button("Login Localhost"))
		{
			m_client.ConnectToServer("", buf);

			m_loggedIn = true;
		}
	}
	else
	{
		ImGui::Text("Currently logged in as %s", buf);
		ImGui::Text("Currently connected to server at %s", buf_2);

		if (ImGui::Button("Disconnect"))
		{
			m_client.DisconnectFromServer();

			m_loggedIn = false;
		}
	}


	ImGui::Separator();

	//Print out list of other clients
	if (m_loggedIn)
	{
		for (int i = 0; i < m_client.m_otherClients.size(); i++)
		{
			int otherID = m_client.m_otherClients[i].m_clientID;

			std::string chatTitle = (m_client.m_chatActivity.GetRequestSent(otherID)) ? "Request Sent" : "Chat";
			std::string gameTitle = (m_gameRequestSent) ? "Request Sent" : "Game";


			//Prints out client and username
			ImGui::Text("Client %i : %s %s", m_client.m_otherClients[i].m_clientID, m_client.m_otherClients[i].m_username.c_str(), PrintActivity::Return(m_client.m_otherClients[i].m_activity).c_str());
			ImGui::SameLine();
			//Did you receive a chat request?
			if (m_client.m_chatActivity.GetRequestReceived(otherID))
			{
				ImGui::SameLine();
				if (ImGui::Button("Accept Request"))
				{
					//Send accept response
					m_client.SendMsg(MessageType::MSG_CHATRESPONSE, &ChatResponse(otherID, true), MessageFlags::NONE);

					m_client.m_chatActivity.RespondRequest(otherID, true);
				}
				ImGui::SameLine();
				if (ImGui::Button("Reject Request"))
				{
					//Send reject response
					m_client.SendMsg(MessageType::MSG_CHATRESPONSE, &ChatResponse(otherID, false), MessageFlags::NONE);

					m_client.m_chatActivity.RespondRequest(otherID, false);
				}
			}
			else
			{
				if (!m_client.m_chatActivity.GetCurrentlyChatting(otherID))
				{
					if (ImGui::Button(std::string(chatTitle + "##" + std::to_string(i)).c_str()))
					{
						if (!m_client.m_chatActivity.GetRequestSent(otherID))
						{
							//Sends a chat request
							m_client.SendMsg(MessageType::MSG_CHATREQUEST, &ChatRequest(m_client.m_clientID, otherID), MessageFlags::NONE);

							//Send chat request
							m_client.m_chatActivity.SendRequest(otherID);
						}
					}
				}
			}

			ImGui::SameLine();
			//Game request stuffs
			if (ImGui::Button(gameTitle.c_str()))
			{
				if (!m_gameRequestSent)
				{
					m_gameRequestSent = true;
				}
			}


			//Create chat window
			if (m_client.m_chatActivity.GetCurrentlyChatting(otherID))
			{
				bool chatting = m_client.m_chatActivity.GetCurrentlyChatting(otherID);
				//Create chat window
				ImGui::Begin(m_client.m_chatActivity.GetChatName(otherID).c_str(), &chatting);

				for (int i = 0; i < m_client.m_chatActivity.GetChatLog(otherID).size(); i++)
				{
					//Print out chat
					ImGui::Text("%s", m_client.m_chatActivity.GetChatLog(otherID)[i].c_str());
				}

				ImGui::Separator();

				ImGui::InputText("", m_client.m_chatActivity.GetChatBufRef(otherID), BUF_LEN);
				ImGui::SameLine();
				if (ImGui::Button("Send"))
				{
					//Send the message
					std::string message = m_client.m_username + ": " + std::string(m_client.m_chatActivity.GetChatBufRef(otherID));
					m_client.SendMsg(MessageType::MSG_CHATSTRING, &ChatMessage(otherID, message), MessageFlags::NONE);
					m_client.m_chatActivity.AddMessage(otherID, message);
					memset(m_client.m_chatActivity.GetChatBufRef(otherID), 0, BUF_LEN);
				}

				if (ImGui::Button("End Chat"))
				{
					m_client.m_chatActivity.EndChat(otherID);
				}

				ImGui::End();
			}

		}
	}

	ImGui::End();
}
