#pragma once

#include <QSet>
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"
#include "ModelColleague.h"

class ServerConnect;
class MainController;

class MainModel : public ModelColleague
{
	Q_OBJECT
public:
	MainModel(IModelManage*);
	~MainModel() override;

    bool Initialize();
	void Uninitialize();

	bool UserLoginWeb(SUserInfo &user);

	bool GetConfigInfo();

	SUserInfo& GetUserInfo();

	bool ConnectTcpServer();

	void LoginTcpServer(unsigned uServerType);

	/*************************************************************************
	* function����סUserKey��Ӧ����Ϣ
	*************************************************************************/
	void LockUserInfo();

	/*************************************************************************
	* function������UserKey��Ӧ����Ϣ
	*************************************************************************/
	void UnlockUserInfo();

	void AddLoginServer(UserKey uUserKey);

	QSet<unsigned> GetLoginServerList();

private slots:

	/*************************����Ϊ�����Զ��ۺ���*************************/
	/*************************************************************************
	* function��		 Tcp����״̬֪ͨ
	* param uServerType: ����������
	* param state:		 ���ӳɹ���ʶ
	* param once:		 ��һ�����ӱ�ʶ
	*************************************************************************/
	void slotTcpConnectNotify(unsigned uServerType, bool bSuccess);
	void slotTcpDisconnectNotify(unsigned uServerType);

private:
	/*************************����Ϊ��Ϣ�ص�����*************************/
	void OnLoginRs(const unsigned uUserKey, SDataExchange* pMsg);

private:
	ServerConnect	*m_pSrvCnnt = nullptr;		//���������
	MainController	*m_pMainCtrl = nullptr;		//

	SUserInfo		m_user;
	MLock			m_lckUserInfo;
	QSet<unsigned>	m_setSrvUser;
	MLock			m_lckSrvUser;
};
