#include "MainController.h"
#include "../../CommonFile/CommonDefine.h"
#include "ControllerColleague.h"
#include "../../MainServer/IMainServer.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../Model/ModelManage/UserInfoManage.h"
#include "../../Component/MessageHandle/IMessageHandle.h"
#include "../../Component/TCPCommunication/ICommunication.h"
#include "../../Component/TCPCommunication/TCPServer.h"
#include <memory>
#include <vector>

MainController::MainController(IControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
}

MainController::~MainController()
{
}

bool MainController::Initialize()
{
	m_pUserMng = m_pModelMng->GetUserInfoManage();
     if(nullptr == m_pUserMng)
     {
		 loge() << "��ȡ��ģ��ʧ��!";
         return false;
     }

	 m_pTcpSrv = m_pCmmnt->GetTCPServer();
	 if (nullptr == m_pTcpSrv)
	 {
		 loge() << "��ȡTCP����ʧ��!";
		 return false;
	 }

    return true;
}

void MainController::Uninitialize()
{
	m_pUserMng = nullptr;
	m_pTcpSrv = nullptr;
}

bool MainController::StartServer()
{
	m_pMsgHandle->RegisterMessageHandle();
	if (false == m_pTcpSrv->StartServer()) return false;
	m_pHeartbeatThread = new std::thread(&MainController::HeartbeatHandle, this);

	return true;
}

void MainController::StopServer()
{
	::SetEvent(m_hHeartbeatEvent);
	if (m_pHeartbeatThread->joinable())
		m_pHeartbeatThread->join();

	m_pTcpSrv->StopServer();
}

void MainController::HeartbeatHandle()
{
	m_hHeartbeatEvent = ::CreateEvent(NULL, false, false, NULL);
	if (NULL == m_hHeartbeatEvent)
	{
		loge() << "���������¼�ʧ�ܣ�";
		return;
	}

	DWORD dwHeartbeatTime = m_pMain->GetServerConfig()->uHeartbeatTime;

	while (true)
	{
		DWORD dwRet = ::WaitForSingleObject(m_hHeartbeatEvent, dwHeartbeatTime);
		if (WAIT_OBJECT_0 == dwRet)
		{
			logm() << "�����˳������̡߳�";
			break;
		}
		else if (WAIT_FAILED == dwRet)
		{
			loge() << "�����̷߳�������" << ::GetLastError();
			break;
		}

		std::vector<unsigned> userList = m_pUserMng->GetOfflineUserList();
		for (unsigned uUserKey : userList)
		{
			m_pTcpSrv->Disconnect(uUserKey);
		}
	}
}

void MainController::HandleLoginRq(const unsigned uUserKey, SDataExchange* pMsg)
{
	std::shared_ptr<SLoginRq> pRq(static_cast<SLoginRq*>(pMsg));
	SUserInfo* pUser = m_pUserMng->GetClientUserInfo(uUserKey);
	if (nullptr == pUser)
	{
		LOGE("��¼�û�key:%d userId:%d userName:%s�����ڡ�", uUserKey, pRq->uUserId, pRq->strUserName);
		return;
	}
	m_pUserMng->LockUserInfo(uUserKey);
	pUser->uUserId = pRq->uUserId;
	pUser->strLoginName = pRq->strUserName;
	pUser->strPassword = pRq->strPassword;
	m_pUserMng->UnlockUserInfo(uUserKey);

	m_pUserMng->AddUserIdUserKeyMap(pRq->uUserId, uUserKey);
	SRespondMsg loginRs;
	loginRs.uResult = 0;
	loginRs.strMsg = "�û���¼�ɹ���";
	m_pTcpSrv->SendData(uUserKey, loginRs.SerializeAsPbMsg());
}

void MainController::HandleHeartbeatNt(const unsigned uUserKey, SDataExchange* pMsg)
{
	std::shared_ptr<SHeartBeatNt> pRq(static_cast<SHeartBeatNt*>(pMsg));
	SUserInfo *pUser = m_pUserMng->GetClientUserInfo(uUserKey);
	if (nullptr == pUser)
	{
		loge() << "�������û�uUserKey��" << uUserKey;
		return;
	}

	m_pUserMng->LockUserInfo(uUserKey);
	pUser->uHeartCount = 0;
	m_pUserMng->UnlockUserInfo(uUserKey);
}
