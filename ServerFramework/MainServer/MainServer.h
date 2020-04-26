#pragma once
#include <string>
#include "IMainServer.h"
#include "../CommonFile/TypeDefine.h"

class MainServer : public IMainServer
{
public:
	MainServer();
	~MainServer();
	SServerConfig* GetServerConfig() override;
	IModelManage* GetModelManage() override;
	IControllerManage* GetControllerManage() override;
	ICommunication* GetCommunication() override;

	bool StartServer();
	void StopServer();

private:
	bool ReadConfigFile();

private:
	SServerConfig		m_srvConfig;		//����������
	IModelManage		*m_pModel;			//ģ�͹���
	IControllerManage	*m_pController;		//������
	ICommunication*		m_pCommunication;	//ͨѶģ��
};

