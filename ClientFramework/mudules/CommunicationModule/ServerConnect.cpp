#include "ServerConnect.h"


ServerConnect::ServerConnect(ICommunication *pCmmnt)
{
	m_pCommunication = pCmmnt;
}

ServerConnect::~ServerConnect()
{

}

bool ServerConnect::Start()
{

}

void ServerConnect::Stop()
{
	m_funSendData = nullptr;		//�������ݻص�
	m_fuDisconnect = nullptr;		//�����Ͽ����ӻص�
	m_mapIdKey.clear();				//�û�id��key��ӳ���ϵ
	m_mapUserList.clear();			//�û�key��UserInfo��ӳ��
}

void ServerConnect::Send(UserKey uUserKey, const char * data, unsigned uLength)
{
	if (m_funSendData)
	{
		m_funSendData(uUserKey, 0, data, uLength);
	}
}

void ServerConnect::Disconnect(UserKey uUserKey)
{
	if (m_fuDisconnect)	m_fuDisconnect(uUserKey);
}

void ServerConnect::AddUser(UserKey uUserKey)
{

}

void ServerConnect::HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length)
{

}

void ServerConnect::DeleteUser(UserKey uUserKey)
{

}
