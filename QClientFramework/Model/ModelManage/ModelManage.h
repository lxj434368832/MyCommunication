#pragma once
/************************************************************************
* filename:ModelManage.h
* function: ģ�͹����������ڹ������ģ��
* author :	������
* datetime:2018-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/

#include "IModelManage.h"

class ModelManage : public IModelManage
{
public:
    ModelManage(IMainClient *_main = nullptr);
    ~ModelManage() override;
    StatisticModel* GetStatisticModel() override;
    MainModel* GetMainModel() override;

	bool Start() override;
	void Stop() override;

private:
    MainModel           *m_mainModel;
    StatisticModel      *m_statistic;
};

