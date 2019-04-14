#pragma once
#include <string>
#include "IMainServer.h"
#include "../CommonFile/TypeDefine.h"

class MainServer : public IMainServer
{
public:
	MainServer();
	~MainServer();
	ServerConfig* GetServerConfig() override;
	IModelManage* GetModelManage() override;
	IControllerManage* GetControllerManage() override;
	IMessageHandle* GetMessageBusiness() override;
	ITCPCommunication* GetCommunication() override;

	bool Start();
	void Stop();

private:
	bool ReadConfigFile();

private:
	ServerConfig		m_srvConfig;		//����������
	IModelManage		*m_pModel;			//ģ�͹���
	IControllerManage	*m_pController;		//������
	IMessageHandle*		m_pMessage;			//��Ϣģ��
	ITCPCommunication*	m_pCommunication;	//ͨѶģ��
};

