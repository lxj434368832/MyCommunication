#pragma once
/*************************************************************************
* function�������ͨѶ����Ľӿڣ����е�ͨѶ���ڴ�ģ���й���
* author :	������
* datetime��2017-12-14
* company:
*************************************************************************/

#include <QObject>
#include "..\..\CommonFile\TypeDefine.h"

class IMainClient;
class ServerConnect;

class ICommunication : public QObject
{
	Q_OBJECT
public:
    ICommunication(IMainClient *main = nullptr):m_pMain(main){}
    virtual ~ICommunication() { m_pMain = nullptr; }
	inline IMainClient* GetMainClient() { return m_pMain; }
	virtual ServerConnect* GetServerConnect() = 0;

	virtual bool Initialize() = 0;
	virtual void Uninitialize() = 0;
	virtual bool ConnectServer() = 0;

signals:
	void signalTcpConnectNotify(unsigned uServerType, bool bSuccess);
	void signalTcpDisconnectNotify(unsigned uServerType);

protected:
	IMainClient *m_pMain;
};
