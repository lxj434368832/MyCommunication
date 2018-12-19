#pragma once

#include "TypeDefine.h"
#include "IMainClient.h"

class IOCPClient;
class INetInterface;

class MainClient : public IMainClient
{
public:
	MainClient();
	~MainClient();

	bool Start() override;
	void Stop() override;

private:
	bool ReadConfigFile();

private:
	ClientConfig	m_srvConfig;	//����������
	INetInterface	*m_pClientList;	//�ͻ����б�
	IOCPClient		*m_pIOCPClient;	//IOCP �ͻ���
};

