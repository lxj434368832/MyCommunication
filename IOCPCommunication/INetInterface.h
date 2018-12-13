#pragma once

#include <map>
#include <functional>

typedef std::function<void(unsigned, unsigned, const char*, unsigned)> SendDataCallback;
typedef std::function<void(unsigned)>	DisconnectCallback;

typedef unsigned UserKey;
typedef unsigned UserId;

struct UserInfo
{
	UserKey	m_uUserKey = 0;
	UserId m_uUserId = 0;
};

class INetInterface
{
public:
	INetInterface();
	virtual ~INetInterface();

private:
	//ע��ص�����
	void SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect);

	//�·����û��ɹ�����֪ͨ
	virtual void AddUser(unsigned uUserKey);

	//�����������
	virtual void HandData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length);

	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	virtual void DeleteUser(unsigned uUserKey);

protected:
	friend class IOCPBase;
	SendDataCallback	m_funSendData;		//�������ݻص�
	DisconnectCallback	m_fuDisconnect;		//�����Ͽ����ӻص�
	std::map<UserId, UserKey> m_mapIdKey;	//�û�id��key��ӳ���ϵ
	std::map<UserKey, UserInfo*> m_mapUserList;	//�û�key��UserInfo��ӳ��
};

