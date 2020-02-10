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
class ICommunication;
class IMessageHandle; 
struct SPbMsg;

//#define CONNECT_SHARE_LOCK_COUNT 16

class ServerConnect : public INetInterface
{
public:
	ServerConnect(ICommunication *pCmmnt = nullptr);
	virtual ~ServerConnect();
	bool Initialize(unsigned uThreadCount);
	void Uninitialize();

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
	* param uMsgType: ��Ϣ����
	* param msg:	 ��Ҫ���͵�����
	*************************************************************************/
	void SendData(UserKey uUserKey, SPbMsg &msg);

	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	/*************************************************************************
	* function�� �Ͽ����ӣ��ײ��ص�DeleteUser����
	* param key: �û�id
	* return:	 ��
	*************************************************************************/
	void Disconnect(unsigned uUserKey);

private:
	//ʵ��INetInterface�ӿ�
	//���ӽ��֪ͨ
	void ConnectNotify(UserKey uUserKey, bool bSuccess) override;

	//�����������
	void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// ɾ���û�
	void DeleteUser(unsigned uUserKey) override;

private:
	ICommunication*	m_pCmmnt;
	IMessageHandle*		m_pMsgModule;
	IOCPClient			*m_pIOCPClient;	//IOCP �ͻ���

};

