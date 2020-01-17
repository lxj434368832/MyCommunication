#pragma once
/*************************************************************************
* function����Ϣ����ͬ���࣬������Ҫ������Ϣ���඼�̳д���,��Ҫ���ڴ洢��Ҫ���ⲿ��
* author :	������
* datetime��2018-12-14
* company:
*************************************************************************/

class IMessageHandle;
class ITCPCommunication;
class MainController;

class IMessageColleague
{
public:
	IMessageColleague(IMessageHandle *pHandle = nullptr);
	virtual ~IMessageColleague();

	bool Start();
	void Stop();

protected:
	IMessageHandle		*m_pMsgHandle = nullptr;
	ITCPCommunication	*m_pTcpCmmnt = nullptr;
	MainController		*m_pMainCtrl = nullptr;		//��������
};