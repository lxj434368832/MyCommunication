#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: ������ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

class HandleRequestMessage;
class HandleRespondMessage;
class HandleNotifyMessage;
class ITCPCommunication;
class IMainServer;
class IControllerManage;
class IModelManage;

class ControllerColleague
{
public:
    ControllerColleague(IControllerManage*);
    virtual ~ControllerColleague();

protected:
	IMainServer             *m_pMain = nullptr;
    IControllerManage		*m_pCtrlMng = nullptr;
	IModelManage			*m_pModelMng = nullptr;
	ITCPCommunication		*m_pTcpCmmnt = nullptr;
	HandleRequestMessage	*m_pHandleRqMsg = nullptr;
	HandleRespondMessage	*m_pHandleRsMsg = nullptr;
	HandleNotifyMessage		*m_pHandleNtMsg = nullptr;
};
