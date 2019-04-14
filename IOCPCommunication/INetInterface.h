#pragma once

#include <map>
#include <functional>
#include "..\Framework\include\MLock.h"

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

	//ע��ص�����
	void SetNetworkCallback(SendDataCallback funSendData, DisconnectCallback funDisconnect);

	//�·����û��ɹ�����֪ͨ
	virtual void AddUser(UserKey uUserKey);

	//�����������
	virtual void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length);

	//ͨѶ��ص�ɾ���û�
	virtual void DeleteUser(UserKey uUserKey);

protected:

	SendDataCallback				m_funSendData;		//�������ݻص�
	DisconnectCallback				m_fuDisconnect;		//�����Ͽ����ӻص�

	//std::map<UserId, UserKey>		m_mapIdKey;			//�û�id��key��ӳ���ϵ
	//MLock							m_lckIdKey;			//�û�id��key��ӳ���ϵ��
	//std::map<UserKey, UserInfo*>	m_mapUserList;		//�û�key��UserInfo��ӳ��
	//MLock							m_lckUserList;		//�û��б���
};

