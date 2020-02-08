#pragma once

#include "IModelManage.h"

class ModelManage : public IModelManage
{
public:
	ModelManage(IMainServer* pMain = nullptr);
	~ModelManage();
	MainModel* GetMainModel() override;
	UserInfoManage* GetUserInfoManage() override;

	bool Initialize() override;
	void Uninitialize() override;

private:
	MainModel		*m_pMainModel;		//��ģ��
	UserInfoManage	*m_pUserInfo;
};

