#pragma once
/*************************************************************************
* function�������ͨѶ����Ľӿڣ����е�ͨѶ�������ݿ⽻�����ڴ�ģ���й���
* author :	������
* datetime��2017-12-14
* company:
*************************************************************************/

#include "..\..\CommonFile\TypeDefine.h"

class IMainServer;

class ICommunication
{
public:
	ICommunication(IMainServer *pMain = nullptr):m_pMain(pMain){};
	virtual ~ICommunication() { m_pMain = nullptr; };
	virtual TCPServer* GetTCPServer() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

	virtual bool StartServer() = 0;
	virtual void StopServer() = 0;

protected:
	IMainServer *m_pMain;
};
