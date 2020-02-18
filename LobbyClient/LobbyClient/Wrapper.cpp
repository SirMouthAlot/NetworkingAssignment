//#include "Wrapper.h"
//
//Client myClient;
//
//PLUGIN_API void InitClient()
//{
//	return myClient.InitClient();
//}
//
//PLUGIN_API void ConnectToServer(const char* ip)
//{
//	return myClient.ConnectToServer(ip);
//}
//
//PLUGIN_API void SendFloat(float flt, MessageFlags flag)
//{
//	Float temp(flt);
//	return myClient.SendMsg(MessageType::MSG_FLOAT, &temp, flag);
//}
//
//PLUGIN_API void SendInt(int it, MessageFlags flag)
//{
//	Int temp(it);
//	return myClient.SendMsg(MessageType::MSG_INT, &temp, flag);
//}
//
//PLUGIN_API void SendString(const char* str, MessageFlags flag)
//{
//	String temp(str);
//	return myClient.SendMsg(MessageType::MSG_STRING, &temp, flag);
//}
//
//PLUGIN_API void SendVector(Vector3CS vec, MessageFlags flag)
//{
//	Vector3 temp = Vector3(vec.x, vec.y, vec.z);
//	return myClient.SendMsg(MessageType::MSG_VECTOR3, &temp, MessageFlags::BROADCAST_ALL);
//}
//
//PLUGIN_API float RecvFloat()
//{
//	Float temp;
//	MessageType type;
//	myClient.RecvMsg(type, &temp);
//
//	return temp.m_float;
//}
//
//PLUGIN_API int RecvInt()
//{
//	Int temp;
//	MessageType type;
//	myClient.RecvMsg(type, &temp);
//
//	return temp.m_int;
//}
//
//PLUGIN_API const char* RecvString()
//{
//	String temp;
//	MessageType type;
//	myClient.RecvMsg(type, &temp);
//	
//	const char* test = temp.m_string.c_str();
//	return test;
//}
//
//PLUGIN_API Vector3CS RecvVector()
//{
//	Vector3 temp;
//	MessageType type;
//	myClient.RecvMsg(type, &temp);
//	
//	Vector3CS csVersion = Vector3CS(temp.x, temp.y, temp.z);
//
//	return csVersion;
//}
//
//PLUGIN_API void DisconnectFromServer()
//{
//	myClient.DisconnectFromServer();
//}
//
//PLUGIN_API void CloseClient()
//{
//	myClient.ShutdownClient();
//}
