#pragma once
/*************************************************************************
* function����Ϣ����ͬ���࣬������Ҫ������Ϣ���඼�̳д���
* author :	������
* datetime��2018-12-14
* company:
*************************************************************************/
class IMessageHandle;

class IMessageColleague
{
public:
	IMessageColleague(IMessageHandle *pHandle = nullptr){
		m_pMsgHandle = pHandle;
	}
	virtual ~IMessageColleague(){
		m_pMsgHandle = nullptr;
	}

protected:
	IMessageHandle *m_pMsgHandle;
};