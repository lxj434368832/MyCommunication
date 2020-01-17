#pragma once
/************************************************************************
* filename:MainController.h
* function:�������������ڳ��������Լ��û���½ҵ��Ĵ���
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include "ControllerColleague.h"
#include "../../CommonFile/TypeDefine.h"

class UserInfoManage;

class MainController : public ControllerColleague
{
public:
    explicit MainController(IControllerManage*);
    ~MainController() override;

    bool Start();
    void Stop();

	void UserLogin(ClientUserInfo &userInfo);
private:
	UserInfoManage   *m_pUserMng;
};

