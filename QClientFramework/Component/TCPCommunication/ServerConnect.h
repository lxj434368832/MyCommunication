#pragma once
/*************************************************************************
* function����������ӹ���
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/
#include "../../3rdParty/IOCPCommunication/include/INetInterface.h"

class MessageHandle;
class IOCPClient;
class ITCPCommunication;
class IMessageHandle;

//#define CONNECT_SHARE_LOCK_COUNT 16

class ServerConnect : public INetInterface
{
public:
	ServerConnect(ITCPCommunication *pCmmnt = nullptr);
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

	/*************************************************************************
	* function�� �Ͽ����ӣ��ײ��ص�DeleteUser����
	* param key: �û�id
	* return:	 ��
	*************************************************************************/
	void Disconnect(UserKey uUserKey);

private:
	//ʵ��INetInterface�ӿ�
	//�·����û��ɹ�����֪ͨ
	void AddUser(UserKey uUserKey) override;

	//�����������
	void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// ɾ���û�
	void DeleteUser(UserKey uUserKey) override;

private:
	ITCPCommunication*	m_pCommunication;
	IMessageHandle*		m_pMsgModule;
	IOCPClient			*m_pIOCPClient;	//IOCP �ͻ���

};

