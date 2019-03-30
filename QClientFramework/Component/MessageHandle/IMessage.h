#pragma once
/*************************************************************************
* function���������Ϣ����ӿ�
* author :	������
* datetime��2018-12-14
* company:
*************************************************************************/

class IMainClient;
class ProtobufMsgFactory;
class LoginMessageHandle;

class IMessage
{
public:
	IMessage(IMainClient *main = nullptr):m_main(main){ }
	virtual ~IMessage() { m_main = nullptr; };
	inline IMainClient* GetMainClient() { return m_main; }
	virtual ProtobufMsgFactory* GetProtobufMsgFactory() = 0;
	virtual LoginMessageHandle* GetLoginMessageHandle() = 0;
	virtual bool Start() = 0;
	virtual void Stop() = 0;

	//��protobuf����
	virtual void HandleProtobufMessage(unsigned uUserKey, const char* data, unsigned length) = 0;

protected:
	IMainClient *m_main;
};
