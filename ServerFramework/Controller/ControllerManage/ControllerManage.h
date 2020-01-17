#pragma once
/************************************************************************
* filename:ControllerManage.h
* function: �������������ڹ������������
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/


#include "IControllerManage.h"

class ControllerManage : public IControllerManage
{
public:
    ControllerManage(IMainServer *_main = nullptr);
	~ControllerManage() override;

    MainController* GetMainController() override;

	bool Start() override;
	void Stop() override;

private:
    MainController      *m_pMainCtrl = nullptr;
};

