#pragma once
/*************************************************************************
* function����Ϣ����ӿڻ��࣬������Ҫ������Ϣ���඼�̳д���
* author :	������
* datetime��2018-12-14
* company:
*************************************************************************/
class IMessage;

class IMessageHandle
{
public:
	IMessageHandle(IMessage *pMsg = nullptr){
		m_pMsgModule = pMsg;
	}
	virtual ~IMessageHandle(){
		m_pMsgModule = nullptr;
	}

protected:
	IMessage *m_pMsgModule;
};