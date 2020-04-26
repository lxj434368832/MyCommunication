#pragma once

#include <set>
#include "IMessageHandle.h"

class PbMessageHandle;

class MessageHandle : public IMessageHandle
{
public:
	MessageHandle(IMainServer *pMain = nullptr);
	~MessageHandle();
	PbMessageHandle* GetPbMessageHandle() override;
	HandleRequestMessage* GetHandleRequestMessage() override;
	HandleRespondMessage* GetHandleRespondMessage() override;
	HandleNotifyMessage* GetHandleNotifyMessage() override;

	bool Initialize() override;
	void Uninitialize() override;

	void RegisterMessageHandle(unsigned, std::function<void(const unsigned uUserKey, SDataExchange*)>) override;
	//����protobuf���ݣ���ͨѶģ�����
	void HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

private:
	PbMessageHandle					*m_pPbMsgHandle;
	HandleRequestMessage			*m_pHandleRqMsg;
	HandleRespondMessage			*m_pHandleRsMsg;
	HandleNotifyMessage				*m_pHandleNtMsg;
};

