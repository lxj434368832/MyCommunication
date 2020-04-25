#pragma once

#include <set>
#include "IMessageHandle.h"

class PbMessageHandle;

class MessageHandle : public IMessageHandle
{
public:
	MessageHandle(IMainClient *pMain = nullptr);
	~MessageHandle();
	PbMessageHandle* GetPbMessageHandle() override;

	bool Initialize() override;
	void Uninitialize() override;

	//����protobuf���ݣ���ͨѶģ�����
	void HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length) override;

private:
	PbMessageHandle				*m_pPbMessageHandle;
};

