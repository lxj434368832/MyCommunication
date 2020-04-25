#pragma once
/************************************************************************
* filename:ModelColleague.h
* function: ģ��ͬ�ˣ����ڸ����ӿ������̳�
* author :	������
* datetime:2020-4-24
* company:
/************************************************************************/

#include <QObject>

class IModelManage;
class IMainClient;
class IMessageHandle;
class ICommunication;

class ModelColleague : public QObject
{
public:
	ModelColleague(IModelManage *, QObject *parent = nullptr);
	virtual ~ModelColleague();

protected:
	IMainClient             *m_pMain = nullptr;
	IModelManage			*m_pMdlMng = nullptr;
	ICommunication			*m_pCmmnt = nullptr;
	IMessageHandle			*m_pMsgHandle = nullptr;
};

