#pragma once

#include "../CommonFile/TypeDefine.h"
#include "IMainClient.h"

class MainClient : public IMainClient
{
public:
	MainClient();
    ~MainClient();
	ClientConfig* GetClientConfig() override;
	MConfigManage* GetConfigManage() override;
	IViewManage* GetViewManage() override;
	IControllerManage* GetControllerManage() override;
	IModelManage* GetModelManage() override;

    ITCPCommunication* GetTCPCommunication() override;
	IMessageHandle*	GetMessageHandle() override;

	bool Start() override;
	void Stop() override;

private:
	bool ReadConfigFile();

private:
	ClientConfig		m_clConfig;	//�ͻ���������
	MConfigManage*		m_pCfgMng;	//���ù���
	IViewManage*		m_pView;
    IControllerManage*	m_pController;
	IModelManage*		m_pModel;

    ITCPCommunication*		m_pTCPCommunication;
    IMessageHandle*         m_pMessage;
};

