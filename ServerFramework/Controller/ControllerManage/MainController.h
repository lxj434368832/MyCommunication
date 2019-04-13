#pragma once
/************************************************************************
* filename:MainController.h
* function:�������������ڳ��������Լ��û���½ҵ��Ĵ���
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include "ControllerColleague.h"

class MainModel;

class MainController : public ControllerColleague
{
public:
    explicit MainController(IMainServer*);
    ~MainController() override;

    bool Start();
    void Stop();

private:
    MainModel   *m_mainModel;
};

