#pragma once
/*************************************************************************
* function�������ͨѶ����Ľӿڣ����е�ͨѶ�������ݿ⽻�����ڴ�ģ���й���
* author :	������
* datetime��2017-12-14
* company:
*************************************************************************/

#include <functional>
#include "..\CommonFile\TypeDefine.h"

class IMainServer;
class PbMessageHandle;
class TCPServer;

class ICommunication
{
public:
	ICommunication(IMainServer *pMain = nullptr):m_pMain(pMain){};
	virtual ~ICommunication() { m_pMain = nullptr; };
	virtual PbMessageHandle* GetPbMessageHandle() = 0;
	virtual TCPServer* GetTCPServer() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;

	virtual void RegisterMessageHandle(unsigned, std::function<void(const unsigned, SDataExchange*)>) = 0;

protected:
	IMainServer *m_pMain;
};
