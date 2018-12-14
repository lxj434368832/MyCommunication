#pragma once
/*************************************************************************
* function�����ӹ������ļ�
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/
#include "../IOCPCommunication/INetInterface.h"

class MessageBusiness;
struct UserInfo;

//�ͻ��˵�UserKey��ʹ�ô�ö�ٱ���
enum class EServerType
{
	EST_UNKNOWN,
	EST_CMD_SERVER,
	EST_DATA_SERVER
};

class ClientManage : public INetInterface
{
public:
	ClientManage();
	virtual ~ClientManage();
	void Start();
	void Stop();
	

private:
	//�·����û��ɹ�����֪ͨ
	virtual void AddUser(unsigned uUserKey) override;

	//�����������
	virtual void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	virtual void DeleteUser(unsigned uUserKey) override;

};

