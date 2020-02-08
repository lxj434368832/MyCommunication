#pragma once
/*************************************************************************
* function������Ĺ������Ľӿڣ���Ҫ�����е����ݽ��д洢����
* author :	������
* datetime��2018-12-14
* company:
*************************************************************************/
class IMainServer;
class UserInfoManage;
class MainModel;

class IModelManage
{
public:
	IModelManage(IMainServer* pMain = nullptr):m_pMain(pMain){}
	virtual ~IModelManage() { m_pMain = nullptr; }
	virtual MainModel* GetMainModel() = 0;
	virtual UserInfoManage* GetUserInfoManage() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

protected:
	IMainServer*	m_pMain;

};
