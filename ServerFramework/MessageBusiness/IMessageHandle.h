#pragma once
/*************************************************************************
* function����Ϣ����ӿڻ��࣬������Ҫ������Ϣ���඼�̳д���
* author :	������
* datetime��2018-12-14
* company:
*************************************************************************/
class IMessageBusiness;

class IMessageHandle
{
public:
	IMessageHandle(IMessageBusiness *pBusiness = nullptr){
		m_pMsgBusiness = pBusiness;
	}
	virtual ~IMessageHandle(){
		m_pMsgBusiness = nullptr;
	}

protected:
	IMessageBusiness *m_pMsgBusiness;
};