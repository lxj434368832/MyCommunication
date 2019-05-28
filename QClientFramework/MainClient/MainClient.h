#pragma once

#include "../CommonFile/TypeDefine.h"
#include "IMainClient.h"

class MainClient : public IMainClient
{
public:
	MainClient();
    ~MainClient() override;
	virtual ClientConfig* GetClientConfig() override;
	virtual IViewManage* GetViewInterface() override;
	virtual IControllerManage* GetControllerInterface() override;
	virtual IModelManage* GetModelInterface() override;

    virtual ITCPCommunication* GetTCPCommunication() override;
	virtual IMessageHandle*	GetMessageHandle() override;

	bool Start() override;
	void Stop() override;

private:
	bool ReadConfigFile();

private:
	ClientConfig		m_clConfig;	//�ͻ���������
	IViewManage*		m_pView;
    IControllerManage*	m_pController;
	IModelManage*		m_pModel;

    ITCPCommunication*		m_pTCPCommunication;
    IMessageHandle*         m_pMessage;
};

