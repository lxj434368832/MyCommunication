#pragma once
#include "IViewManage.h"

class ViewMediator;
class MMainWindow;

class ViewManage : public IViewManage
{
public:
	ViewManage(IMainClient *_main = nullptr);
    ~ViewManage() override;

	bool Start() override;
	void Stop() override;

private:
    ViewMediator    *m_viewMdt;        //��ͼ�н���
    MMainWindow     *m_mainWnd;        //������
};

