#pragma once
/*************************************************************************
* function������Ĺ������Ľӿڣ���Ҫ�����е����ݽ��д洢����
* author :	������
* datetime��2018-12-14
* company:
*************************************************************************/
class IMainServer;
class UserInfoManage;

class IModelManage
{
public:
	IModelManage(IMainServer* srv = nullptr):m_pSrv(srv){}
	virtual ~IModelManage() { m_pSrv = nullptr; }
	virtual IMainServer* GetMainServer() { return m_pSrv; }

	virtual UserInfoManage* GetUserInfoManage() = 0;

	virtual bool Start() = 0;
	virtual void Stop() = 0;

protected:
	IMainServer*	m_pSrv;

};
