#include "MainController.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/MainModel.h"
#include "../../Component/MessageHandle/IMessageHandle.h"
#include "../../Component/TCPCommunication/ICommunication.h"
#include "../../Component/TCPCommunication/ServerConnect.h"
#include "../../Component/Utility/NetworkHelp.h"
#include "../../MainClient/IMainClient.h"
#include <memory>

MainController::MainController(ControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	//�����źźͲ�
	connect(this, SIGNAL(signalUserLogin(std::string, std::string)), SLOT(slotUserLogin(std::string, std::string)));
	connect(this, SIGNAL(signalExecuteSystem()), SLOT(slotExecuteSystem()));
    connect(m_pCmmnt, SIGNAL(signalTcpConnectNotify(uint,bool)), SLOT(slotTcpConnectNotify(uint,bool)));
    connect(m_pCmmnt,SIGNAL(signalTcpDisconnectNotify(uint)),SLOT(slotTcpDisconnectNotify(uint)));

	//�����첽�߳�
    moveToThread(&m_thread);
    m_thread.start();

	qRegisterMetaType<std::string>("std::string");
}

MainController::~MainController()
{
	m_thread.quit();
	if (false == m_thread.wait(3000))
	{
		loge() << "���������߳��˳��ȴ���ʱ��";
		m_thread.terminate();
	}

	m_pMainModel = nullptr;
}

bool MainController::Initialize()
{
    m_pMainModel = m_pModel->GetMainModel();
     if(nullptr == m_pMainModel)
     {
         loge()<< "��ȡ��ģ��ʧ��!";
         return false;
     }

	 m_pSrvCnnt = m_pCmmnt->GetServerConnect();
	 if (nullptr == m_pSrvCnnt)
	 {
		 loge() << "��ȡ���������ʧ��!";
		 return false;
	 }

	 m_pMsgHandle->RegisterMessageHandle();
	 m_pHeartbeatThread = new std::thread(&MainController::HeartbeatHandle, this);

    return true;
}

void MainController::Uninitialize()
{
	::SetEvent(m_hHeartbeatEvent);
	if (m_pHeartbeatThread->joinable())
		m_pHeartbeatThread->join();

	m_pMainModel = nullptr;
	m_pSrvCnnt = nullptr;
}

void MainController::slotUserLogin(std::string strUserName, std::string strPassword)
{
	//1����½web
	signalLoginTipMsg("��ʼ��½web��֤�˻���������...");
	std::string strMac = NetworkHelp::GetMACAdress().toLocal8Bit().data();
	ClientConfig *pCfg = m_pMain->GetClientConfig();
	//SUserInfo user = m_pWebCnnt->UserLogin(strUserName, strPassword, strMac);

	SUserInfo user;
	user.m_uUserId = 1;
	user.m_strUserName = strUserName;
	user.m_strPassword = strPassword;

	if (0 == user.m_uUserId)
	{
		emit signalLoginMessageNt(false, "��½webʧ�ܣ�");
		return;
	}
	user.m_strMac = strMac;
	m_pMainModel->SaveUserInfo(user);

	/*//2����ȡ������Ϣ
	signalLoginTipMsg("��ʼ��ȡ�ͻ���������Ϣ...");
	if (false == m_pWebCnnt->GetConfigInfo(strMac, pCfg))
	{
		loge() << "��ȡweb������������Ϣʧ�ܣ�";
		//emit signalLoginMessageNt(false, "��ȡweb������������Ϣʧ�ܣ�");
		//return; ʧ����û��ϵ�����ػ���������
	}*/

	//3�����TCP����
	emit signalLoginTipMsg("��ʼ���ӷ�����...");
	if (false == m_pCmmnt->ConnectServer())
	{
		emit signalLoginMessageNt(false, "���ӷ�����ʧ�ܣ�");
	}
	else
		emit signalLoginMessageNt(true, "���ӷ������ɹ���");
}

void MainController::slotExecuteSystem()
{
	int iUserType = 1;
	//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
	emit signalShowMainWindow(iUserType);
}

void MainController::slotTcpConnectNotify(unsigned uServerType, bool bSuccess)
{
	if (bSuccess)
	{
		LOGM("���ӷ���:%d�ɹ���",uServerType);
		m_pMainModel->AddServerUser(uServerType);
		SLoginRq msg;
		msg.uUserId = uServerType;
		msg.strUserName = "mingqiaowen";
		msg.strPassword = "123456";
		m_pSrvCnnt->SendData(uServerType, msg.SerializeAsPbMsg());
	}
}

void MainController::slotTcpDisconnectNotify(unsigned uServerType)
{
	LOGM("userKey:%d���ӶϿ�",uServerType);
	m_pMainModel->DeleteServerUser(uServerType);
}

void MainController::HeartbeatHandle()
{
	m_hHeartbeatEvent = ::CreateEvent(NULL, false, false, NULL);
	if (NULL == m_hHeartbeatEvent)
	{
		loge() << "���������¼�ʧ�ܣ�";
		return;
	}

	SHeartBeatNt msg;
	msg.uUserId = 1;
	SPbMsg pbMsg = msg.SerializeAsPbMsg();

	DWORD dwHeartbeatTime = m_pMain->GetClientConfig()->uHeartbeatTime;

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

		std::vector<unsigned> userList = m_pMainModel->GetLoginUserList();
		for (unsigned uUserKey : userList)
		{
			m_pSrvCnnt->SendData(uUserKey, pbMsg);
		}
	}
}

void MainController::HandleLoginRs(const unsigned uUserKey, SDataExchange* pMsg)
{
	std::unique_ptr<SRespondMsg> pRs(static_cast<SRespondMsg*>(pMsg));
	SUserInfo *pUser = m_pMainModel->GetServerUser(uUserKey);
	if (nullptr == pUser)
	{
		LOGE("������û�key��%d �����ڡ�");
		return;
	}

	m_pMainModel->LockServerUserInfo(uUserKey);
	pUser->m_bLogin = true;
	m_pMainModel->UnlockServerUserInfo(uUserKey);
}
