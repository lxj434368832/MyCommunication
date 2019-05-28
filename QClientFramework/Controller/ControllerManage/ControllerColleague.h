#pragma once
/************************************************************************
* filename:ControllerColleague.h
* function: ������ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QObject>

class HandleRequestMessage;
class HandleRespondMessage;
class HandleNotifyMessage;
class ITCPCommunication;
class IModelManage;
class ControllerManage;
class IMainClient;

class ControllerColleague : public QObject
{
public:
    ControllerColleague(ControllerManage*, QObject *parent = nullptr);
    virtual ~ControllerColleague();

protected:
    IMainClient             *m_pMain = nullptr;
	ControllerManage		*m_pCtrlMng = nullptr;
	IModelManage			*m_pModel = nullptr; 
	ITCPCommunication		*m_pTcpCmmnt = nullptr;
	HandleRequestMessage	*m_pHandleRqMsg = nullptr;
	HandleRespondMessage	*m_pHandleRsMsg = nullptr;
	HandleNotifyMessage		*m_pHandleNtMsg = nullptr;
};
