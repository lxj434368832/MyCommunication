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
    ControllerManage(IMainClient *_main = nullptr);
    ~ControllerManage() override;
    MainController* GetMainController() override;

	bool Start() override;
	void Stop() override;

	//��½�ɹ�֮���������
	void ExecuteSystem() override;

private:
    MainController      *m_pMainCtrl = nullptr;
};

