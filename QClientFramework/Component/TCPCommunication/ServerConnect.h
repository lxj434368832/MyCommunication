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
	bool Start(unsigned uThreadCount);
	void Stop();

	/*************************************************************************
	* function������һ��������Ҫ�����Ƿ���Ҫ����
	* param ip port: ��Ҫ���ӵ�ip�Ͷ˿ں�
	* param pIO:	 �������ʱҪ�������ݣ���Ҫ����pIO
	* param iRecnnt: �Ƿ�������ʶ,С��0������Ҫ����
	* return:		 ���ش����Ӷ�Ӧ��id,�����������ӳɹ���Ϊ0�������ӳ����˴���
	*************************************************************************/
	virtual bool AddConnect(unsigned uUserKey, std::string ip, ushort port, int iRecnnt = -1);

	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void Send(unsigned uUserKey, const char* data, unsigned uLength);

	/*************************************************************************
	* function�� �Ͽ����ӣ��ײ��ص�DeleteUser����
	* param key: �û�id
	* return:	 ��
	*************************************************************************/
	void Disconnect(unsigned uUserKey);

private:
	//ʵ��INetInterface�ӿ�
	//�·����û��ɹ�����֪ͨ
	void AddUser(unsigned uUserKey) override;

	//�����������
	void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// ɾ���û�
	void DeleteUser(unsigned uUserKey) override;

private:
	ITCPCommunication*	m_pCommunication;
	IMessageHandle*		m_pMsgModule;
	IOCPClient			*m_pIOCPClient;	//IOCP �ͻ���

};

