#pragma once
/*************************************************************************
* function���ͻ��������ӹ���Ķ����ļ�
* author :	������
* datetime��2017-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include "MLock.h"
#include <atomic>
#include <map>

struct UserInfo;

class ClientManage
{
public:
	ClientManage();
	virtual ~ClientManage();

	void SetClientListenSocket(SOCKET listenSocket);

	SOCKET GetClientListenSocket();

	unsigned GetClientUserId();

	//�·����û��ɹ�����֪ͨ
	virtual void ClientAcceptNotify(unsigned uUserId, SOCKET s);

	//�����������
	virtual void HandClientData(unsigned uUserId, char* data, unsigned length);

	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	UserInfo* DeleteClientUser(unsigned userId);

protected:
	virtual UserInfo* CreateClientUserInfo() = 0;

	virtual void ReleaseClientUserInfo(UserInfo* pSrvUserInfo) = 0;

protected:
	SOCKET m_sClientLstnSocket;

	std::map<unsigned, UserInfo*> m_mapClientUserList;	//�ͻ����û��б�
	MLock						 m_lckClientUserList;	//�ͻ����û��б���

private:
	std::atomic<unsigned> m_uClientUidGenerator = 1;

};

