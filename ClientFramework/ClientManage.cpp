#include "ClientManage.h"


ClientManage::ClientManage()
{

}

ClientManage::~ClientManage()
{

}

void ClientManage::Start()
{

}

void ClientManage::Stop()
{
	m_funSendData = nullptr;		//�������ݻص�
	m_fuDisconnect = nullptr;		//�����Ͽ����ӻص�
	m_mapIdKey.clear();				//�û�id��key��ӳ���ϵ
	m_mapUserList.clear();			//�û�key��UserInfo��ӳ��
}

void ClientManage::AddUser(unsigned uUserKey)
{

}

void ClientManage::HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length)
{

}

void ClientManage::DeleteUser(unsigned uUserKey)
{

}
