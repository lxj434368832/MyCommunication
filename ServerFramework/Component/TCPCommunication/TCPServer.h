#pragma once

#include "../../../IOCPCommunication/INetInterface.h"

class IMainServer;
class IOCPServer;
class UserInfoManage;
class IMessageHandle;

class TCPServer : public INetInterface
{
public:
	TCPServer(IMainServer*);
	~TCPServer();

	bool Start();
	void Stop();

	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void SendData(UserKey uUserKey, const char* data, unsigned uLength);

	void Disconnect(UserKey uUserKey);

private:
	//ʵ��INetInterface�ӿ�
	//�·����û��ɹ�����֪ͨ
	virtual void AddUser(UserKey uUserKey) override;

	//�����������
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// ɾ���û�
	virtual void DeleteUser(UserKey uUserKey) override;

private:
	IMainServer			*m_pMain;
	IOCPServer			*m_pIOCPServer;	//IOCP �����
	IMessageHandle		*m_pMsgHandle;
	UserInfoManage		*m_pUserMng;
};

