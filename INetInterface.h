#pragma once

#include <map>
#include <functional>
#include "UserInfo.h"

typedef std::function<void(unsigned, unsigned, const char*, unsigned)> SendDataCallback;
typedef std::function<void(unsigned)>	DisconnectCallback;

class INetInterface
{
public:
	INetInterface();
	virtual ~INetInterface();
	//ע��ص�����
	void SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect);

	//�·����û��ɹ�����֪ͨ
	virtual void AddUser(unsigned uUserKey);

	//�����������
	virtual void HandSrvData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length);

	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	virtual void DeleteUser(unsigned uUserKey);

protected:
	SendDataCallback	m_funSendData;		//�������ݻص�
	DisconnectCallback	m_fuDisconnect;		//�����Ͽ����ӻص�
	std::map<unsigned, UserInfo> m_mapUserList;

};

