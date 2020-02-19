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

	bool Initialize() override;
	void Uninitialize() override;

private:
    MainController      *m_pMainCtrl = nullptr;
};

