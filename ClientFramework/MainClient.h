#pragma once

#include "TypeDefine.h"

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

