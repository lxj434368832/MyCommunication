#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: ������ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

class ICommunication;
class IMainServer;
class IControllerManage;
class IModelManage;

class ControllerColleague
{
public:
    ControllerColleague(IControllerManage*);
    virtual ~ControllerColleague();

protected:
	IMainServer             *m_pMain = nullptr;
	IModelManage			*m_pModelMng = nullptr;
	IControllerManage		*m_pCtrlMng = nullptr;
	ICommunication			*m_pCmmnt = nullptr;
};
