#pragma once
/*************************************************************************
* function�����������ӹ���Ķ����ļ�
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/
#include "../IOCPCommunication/INetInterface.h"

struct UserInfo;

class ServerManage : public INetInterface
{
public:
	ServerManage();
	virtual ~ServerManage();

	//�·����û��ɹ�����֪ͨ
	virtual void AddUser(unsigned uUserKey);

	//�����������
	virtual void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length);

	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	virtual void DeleteUser(unsigned uUserKey);

private:

};
