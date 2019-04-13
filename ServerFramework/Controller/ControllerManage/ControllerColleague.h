#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: ������ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

class IMainServer;
class IControllerManage;
class IModelManage;

class ControllerColleague
{
public:
    ControllerColleague(IMainServer*);
    virtual ~ControllerColleague();

protected:
	IMainServer             *m_main = nullptr;
    IControllerManage *m_controller = nullptr;
    IModelManage        *m_model = nullptr;
};
