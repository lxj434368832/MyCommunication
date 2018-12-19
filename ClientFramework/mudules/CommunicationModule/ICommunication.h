#pragma once
/*************************************************************************
* function�������ͨѶ����Ľӿڣ����е�ͨѶ�������ݿ⽻�����ڴ�ģ���й���
* author :	������
* datetime��2017-12-14
* company:
*************************************************************************/

class IMainClient;

class ICommunication
{
public:
	ICommunication(IMainClient *main = nullptr):m_main(main){};
	virtual ~ICommunication() { m_main = nullptr; };

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainClient *m_main;
};
