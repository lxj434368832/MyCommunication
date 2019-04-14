#pragma once
#include <string>
#include "IMainServer.h"

struct ServerConfig
{
	unsigned short  usListenPort		= 6666;         // �����˿�
	unsigned int    uServerThreadCount	= 4;			// ��ɶ˿ڼ��������߳�����
	unsigned int    uMessageThreadCount = 4;			// ��Ϣ�����߳�����
	unsigned int    uInitAcceptCount	= 10;           // ����˳�ʼ���ܸ���
	unsigned int    uHeartbeatTime		= 6000;			// ��ʱ�������ʱ��ms

	// ��Ҫ�ж϶�ȡ�����������Ƿ���ȷ
	bool CheckValid()
	{
		if (usListenPort == 0
			|| uInitAcceptCount == 0
			|| uServerThreadCount == 0
			|| uHeartbeatTime == 0)
		{
			return false;
		}
		return true;
	}

};

class MainServer : public IMainServer
{
public:
	MainServer();
	~MainServer();
	IMessageHandle* GetMessageBusiness() override;
	IModelManage* GetManageBusiness() override;
	ICommunication* GetCommunication() override;
	ServerConfig* GetServerConfig() override;

	bool Start();
	void Stop();

private:
	bool ReadConfigFile();

private:
	ServerConfig		m_srvConfig;		//����������
	IMessageHandle*		m_pMessage;			//��Ϣģ��
	IModelManage*		m_pModel;			//ģ��ģ��
	ICommunication*		m_pCommunication;	//ͨѶģ��
};

