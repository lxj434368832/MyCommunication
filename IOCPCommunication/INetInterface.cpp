#include "stdafx.h"
#include "INetInterface.h"
#include "..\Framework\include\MLock.h"


INetInterface::INetInterface()
{
	m_funSendData = nullptr;		//�������ݻص�
	m_fuDisconnect = nullptr;		//�����Ͽ����ӻص�
}


INetInterface::~INetInterface()
{
	m_funSendData = nullptr;		//�������ݻص�
	m_fuDisconnect = nullptr;		//�����Ͽ����ӻص�
}

void INetInterface::SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect)
{
	m_funSendData = funSendData;
	m_fuDisconnect = funDisconnect;
}

void INetInterface::AddUser(UserKey uUserKey)
{

}

void INetInterface::HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length)
{

}

void INetInterface::DeleteUser(UserKey uUserKey)
{

}
