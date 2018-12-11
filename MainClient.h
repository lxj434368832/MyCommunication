#pragma once

#include <string>

struct ClientConfig
{
	// ��Ҫ�ж϶�ȡ�����������Ƿ���ȷ
	bool CheckValid()
	{
		if (strServerIP.empty()
			|| usServerPort == 0
			|| uInitAcceptCount == 0
			|| uServerThreadCount == 0
			|| uHeartbeatTime == 0)
		{
			return false;
		}
		return true;
	}

	std::string		strServerIP = "";				//���ӵ�IP
	unsigned short  usServerPort = 6666;			//���Ӷ˿�
	unsigned int    uServerThreadCount = 4;			// ��ɶ˿ڼ��������߳�����
	unsigned int    uMessageThreadCount = 4;		// ��Ϣ�����߳�����
	unsigned int    uInitAcceptCount = 10;          // ����˳�ʼ���ܸ���
	unsigned int    uHeartbeatTime = 6000;			// ��ʱ��������ʱ��ms
};

class IOCPClient;
class INetInterface;

class MainClient
{
public:
	MainClient();
	~MainClient();
	bool Start();
	bool Stop();

private:
	bool ReadConfigFile();

private:
	ClientConfig	m_srvConfig;	//����������
	INetInterface	*m_pClientList;	//�ͻ����б�
	IOCPClient		*m_pIOCPClient;	//IOCP �ͻ���
};

