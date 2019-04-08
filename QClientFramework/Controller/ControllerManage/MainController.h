#pragma once
/************************************************************************
* filename:MainController.h
* function:�������������ڳ��������Լ��û���½ҵ��Ĵ���
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QThread>
#include "ControllerColleague.h"

class MainModel;

class MainController : public ControllerColleague
{
    Q_OBJECT
public:
    explicit MainController(IMainClient*);
    ~MainController() override;

    bool Start();
    void Stop();

signals:

public slots:

private:
    MainModel   *m_mainModel;
    QThread       m_thread;                   //�첽�߳�
};

