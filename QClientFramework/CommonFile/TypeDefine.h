#pragma once

#include <string>

typedef unsigned UserKey;

struct ClientConfig
{
	std::string		strServerIP = "";				//���ӵ�IP
	unsigned short  usServerPort = 6666;			//���Ӷ˿�
	unsigned int    uIOCPThreadCount = 4;			// ��ɶ˿ڼ��������߳�����
	unsigned int    uMessageThreadCount = 4;		// ��Ϣ�����߳�����
	unsigned int    uHeartbeatTime = 6000;			// ��ʱ��������ʱ��ms

	// ��Ҫ�ж϶�ȡ�����������Ƿ���ȷ
    bool CheckValid();
};

struct SUserInfo
{
	unsigned	m_uUserKey;
	unsigned	m_uUserId;
};