#pragma once

#include <map>
#include <functional>

typedef std::function<void(unsigned, unsigned, const char*, unsigned)> SendDataCallback;
typedef std::function<void(unsigned)>	DisconnectCallback;

typedef unsigned UserKey;

class INetInterface
{
public:
	INetInterface();
	virtual ~INetInterface();

	//ע��ص�����
	void SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect);

	//֪ͨ��������û�����
	virtual void AddUser(UserKey uUserKey);
	//���ӽ��֪ͨ
	virtual void ConnectResult(UserKey uUserKey, bool bSuccess);
	//�����������
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length);

	//ͨѶ��ص�ɾ���û�
	virtual void DeleteUser(UserKey uUserKey);

protected:

	SendDataCallback				m_funSendData;		//�������ݻص�
	DisconnectCallback				m_fuDisconnect;		//�����Ͽ����ӻص�
};

