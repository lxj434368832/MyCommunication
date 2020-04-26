#pragma once

#include <functional>
#include "..\..\CommonFile\TypeDefine.h"

class IMainServer;
class PbMessageHandle;
class HandleRequestMessage;
class HandleRespondMessage;
class HandleNotifyMessage;

class IMessageHandle
{
public:
	IMessageHandle(IMainServer *pMain = nullptr):m_pMain(pMain){}
	virtual ~IMessageHandle() { m_pMain = nullptr; }
	inline IMainServer* GetMainServer() { return m_pMain; }
	virtual PbMessageHandle* GetPbMessageHandle() = 0;
	virtual HandleRequestMessage* GetHandleRequestMessage() = 0;
	virtual HandleRespondMessage* GetHandleRespondMessage() = 0;
	virtual HandleNotifyMessage* GetHandleNotifyMessage() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

	virtual void RegisterMessageHandle(unsigned, std::function<void(const unsigned uUserKey, SDataExchange*)>) = 0;
	//����protobuf����
	virtual void HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) = 0;

protected:
	IMainServer*	m_pMain;
};
