#pragma once

typedef unsigned UserKey;

class INetInterface
{
public:
	INetInterface() = default;
	virtual ~INetInterface() = default;

	//֪ͨ��������û�����
	virtual void AddUser(UserKey uUserKey){}

	//���ӽ��֪ͨ
	virtual void ConnectNotify(UserKey uUserKey, bool bSuccess){}

	//�����������
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) = 0;

	//ͨѶ��ص�ɾ���û�
	virtual void DeleteUser(UserKey uUserKey) = 0;
};
