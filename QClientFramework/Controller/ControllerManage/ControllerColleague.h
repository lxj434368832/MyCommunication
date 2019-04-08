#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: ������ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QObject>

class IMainClient;
class IControllerManage;
class IModelManage;

class ControllerColleague : public QObject
{
public:
    ControllerColleague(IMainClient*, QObject *parent = nullptr);
    virtual ~ControllerColleague();

protected:
    IMainClient             *m_main = nullptr;
    IControllerManage *m_controller = nullptr;
    IModelManage        *m_model = nullptr;
};
