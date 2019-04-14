#pragma once
/*************************************************************************
* function�������ͨѶ����Ľӿڣ����е�ͨѶ�������ݿ⽻�����ڴ�ģ���й���
* author :	������
* datetime��2017-12-14
* company:
*************************************************************************/

#include "..\..\CommonFile\TypeDefine.h"

class IMainServer;

class ITCPCommunication
{
public:
	ITCPCommunication(IMainServer *pMain = nullptr):m_pMain(pMain){};
	virtual ~ITCPCommunication() { m_pMain = nullptr; };
	inline IMainServer* GetMainServer() { return m_pMain; }

	virtual bool Start() = 0;
	virtual void Stop() = 0;
	virtual void SendData(UserKey uUserKey, const char* data, unsigned uLength) = 0;

protected:
	IMainServer *m_pMain;
};
