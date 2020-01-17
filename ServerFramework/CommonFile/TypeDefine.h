#pragma once

#include <string>

typedef unsigned UserKey;
typedef unsigned UserId;

struct ClientUserInfo
{
	unsigned	m_uUserKey;
	unsigned	m_uUserId;
	std::string	m_strName;
	std::string m_strPassword;
	std::string m_strSex;
	unsigned	m_unAge;
	std::string m_strFamilyAddress;
	std::string m_strCompanyAddress;

	void Reset();
};

struct ServerConfig
{
	unsigned short  usListenPort = 6666;         // �����˿�
	unsigned int    uServerThreadCount = 4;			// ��ɶ˿ڼ��������߳�����
	unsigned int    uMessageThreadCount = 4;			// ��Ϣ�����߳�����
	unsigned int    uInitAcceptCount = 10;           // ����˳�ʼ���ܸ���
	unsigned int    uHeartbeatTime = 6000;			// ��ʱ�������ʱ��ms

	// ��Ҫ�ж϶�ȡ�����������Ƿ���ȷ
	bool CheckValid();
};
