#pragma once
/*************************************************************************
* function�������ͨѶ����Ľӿڣ����е�ͨѶ�������ݿ⽻�����ڴ�ģ���й���
* author :	������
* datetime��2017-12-14
* company:
*************************************************************************/

class IMainServer;

class ICommunication
{
public:
	ICommunication(IMainServer *srv = nullptr):m_srv(srv){};
	virtual ~ICommunication() { m_srv = nullptr; };

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainServer *m_srv;
};
