#pragma once

struct ClientConfig
{
	// ��Ҫ�ж϶�ȡ�����������Ƿ���ȷ
	bool CheckValid()
	{
		if (usListenPort == 0
			|| uInitAcceptCount == 0
			|| uServerThreadCount == 0
			|| uHeartbeatTime == 0
			|| strServerIP.empty()
			|| usServerPort == 0)
		{
			return false;
		}
		return true;
	}

	unsigned short  usListenPort		= 6666;         // �����˿�
	unsigned int    uServerThreadCount	= 4;			// ��ɶ˿ڼ��������߳�����
	unsigned int    uMessageThreadCount = 4;			// ��Ϣ�����߳�����
	unsigned int    uInitAcceptCount	= 10;           // ����˳�ʼ���ܸ���
	unsigned int    uHeartbeatTime		= 6000;			// ��ʱ�������ʱ��ms
	std::string		strServerIP			= "";			//���ӵ�IP
	unsigned short  usServerPort		= 6666;			//���Ӷ˿�
};

class IOCPServier;
class INetInterface;

class MainServer
{
public:
	MainServer();
	~MainServer();
	bool Start();
	bool Stop();

private:
	bool ReadConfigFile();

private: 
	ClientConfig	m_srvConfig;	//����������
	INetInterface	*m_pClientList;	//�ͻ����б�
	IOCPServier		*m_pIOCPServer;	//IOCP �����
};

