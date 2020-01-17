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

class ITCPCommunication : public QObject
{
	Q_OBJECT
public:
    ITCPCommunication(IMainClient *main = nullptr):m_pMain(main){}
    virtual ~ITCPCommunication() { m_pMain = nullptr; }
	inline IMainClient* GetMainClient() { return m_pMain; }

	virtual bool Start() = 0;
	virtual void Stop() = 0;

	virtual bool ConnectServer() = 0;

	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	virtual void SendData(UserKey uUserKey, const char* data, unsigned uLength) = 0;

signals:
	void signalTcpConnectNotify(unsigned uServerType, bool bSuccess = true);
	void signalTcpDisconnectNotify(unsigned uServerType);

protected:
	IMainClient *m_pMain;
};
