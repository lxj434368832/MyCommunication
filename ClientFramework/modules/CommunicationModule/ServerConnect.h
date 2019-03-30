#pragma once
/*************************************************************************
* function����������ӹ���
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/
#include "../../../IOCPCommunication/INetInterface.h"
#include "../../../Framework/include/ResourceManage.h"
#include "..\..\include\EnumDefine.h"

class MessageModule;
class IOCPClient;
class ICommunication;
class IMessage;

//#define CONNECT_SHARE_LOCK_COUNT 16

class ServerConnect : public INetInterface
{
public:
	ServerConnect(ICommunication *pCmmnt = nullptr);
	virtual ~ServerConnect();
	bool Start();
	void Stop();

	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void Send(UserKey uUserKey, const char* data, unsigned uLength);

	void Disconnect(UserKey uUserKey);

private:
	//ʵ��INetInterface�ӿ�
	//�·����û��ɹ�����֪ͨ
	virtual void AddUser(UserKey uUserKey) override;

	//�����������
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	virtual void DeleteUser(UserKey uUserKey) override;

private:
	ICommunication*		m_pCommunication;
	IMessage*			m_pMsgModule;
	IOCPClient			*m_pIOCPClient;	//IOCP �ͻ���

	mqw::ResourceManage<UserInfo>	m_rscUser;
	//MLock				m_shareLock[CONNECT_SHARE_LOCK_COUNT];
};

