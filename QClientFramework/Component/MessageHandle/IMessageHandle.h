#pragma once

class IMainClient;
class PbMessageHandle;
class HandleRequestMessage;
class HandleRespondMessage;
class HandleNotifyMessage;

class IMessageHandle
{
public:
	IMessageHandle(IMainClient *pMain = nullptr):m_pMain(pMain){}
	virtual ~IMessageHandle() { m_pMain = nullptr; }
	inline IMainClient* GetMainClient() { return m_pMain; }
	virtual PbMessageHandle* GetProtobufMsgFactory() = 0;
	virtual HandleRequestMessage* GetHandleRequestMessage() = 0;
	virtual HandleRespondMessage* GetHandleRespondMessage() = 0;
	virtual HandleNotifyMessage* GetHandleNotifyMessage() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

	virtual void RegisterMessageHandle() = 0;
	//����protobuf����
	virtual void HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) = 0;

protected:
	IMainClient*	m_pMain;
};
