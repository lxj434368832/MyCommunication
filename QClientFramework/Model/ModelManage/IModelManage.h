#pragma once

class IMainClient;
class MainModel;
class ICommunication;
class IMessageHandle;

class IModelManage
{
public:
	IModelManage(IMainClient *_main = nullptr) :m_pMain(_main) {}
	virtual ~IModelManage() { m_pMain = nullptr; }
	IMainClient* GetMainClient() { return m_pMain; }
	virtual MainModel* GetMainModel() = 0;
	virtual ICommunication* GetCommunication() = 0;
	virtual IMessageHandle*	GetMessageHandle() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

protected:
	IMainClient *m_pMain = nullptr;
};
