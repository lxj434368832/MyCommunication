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

	 m_pHeartbeatThread = new std::thread(&MainController::HeartbeatHandle, this);

    return true;
}

void MainController::Uninitialize()
{
	::SetEvent(m_hHeartbeatEvent);
	if (m_pHeartbeatThread->joinable())
		m_pHeartbeatThread->join();
	RELEASE(m_pHeartbeatThread);
	RELEASE_HANDLE(m_hHeartbeatEvent);
}

void MainController::slotUserLogin(std::string strUserName, std::string strPassword)
{
	//1����½web
	emit signalLoginTipMsg("��ʼ��½web��֤�˻���������...");
	std::string strMac = NetworkHelp::GetMACAdress().toLocal8Bit().data();

	SUserInfo user;
	user.m_strUserName = strUserName;
	user.m_strPassword = strPassword;
	user.m_strMac = strMac;
	if (false == m_pMainModel->UserLoginWeb(user))
	{
		emit signalLoginMessageNt(false, "��½webʧ�ܣ�");
		return;
	}

	//2����ȡ������Ϣ
	signalLoginTipMsg("��ʼ��ȡ�ͻ���������Ϣ...");
	if (false == m_pMainModel->GetConfigInfo())
	{
		loge() << "��ȡweb������������Ϣʧ�ܣ�";
		//emit signalLoginMessageNt(false, "��ȡweb������������Ϣʧ�ܣ�");
		//return; ʧ����û��ϵ�����ػ���������
		}
	emit signalLoginMessageNt(true, "���ӷ������ɹ���");
}

void MainController::slotExecuteSystem()
{
	//3�����TCP����
	emit signalSplashMessage("��ʼ���ӷ�����...");
	int iUserType = 1;
	if (false == m_pMainModel->ConnectTcpServer())
	{
		emit signalSplashMessage("���ӷ�����ʧ�ܣ�");
		emit signalShowMainWindow(iUserType);		//�˴����ݾ����ҵ������
	}
	else
		emit signalShowMainWindow(iUserType);
	//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
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

		QSet<unsigned> setSrvKey = m_pMainModel->GetLoginServerList();
		for (unsigned uSrvKey : setSrvKey)
		{
			//m_pSrvCnnt->SendData(uSrvKey, pbMsg);
		}
	}
}

void MainController::HandleTcpConnectNotify(unsigned uServerType, bool bSuccess)
{
	if (bSuccess)
	{
		LOGM("���ӷ���Key:%d�ɹ���", uServerType);

		m_pMainModel->LoginTcpServer(uServerType);
	}
	else
	{
		loge() << "���ӷ���uServerType:" << uServerType << "ʧ�ܣ�";
	}
}

void MainController::HandleTcpDisconnectNotify(unsigned uServerType)
{
	LOGM("����Key:%d�����ӶϿ�", uServerType);
}

void MainController::HandleLoginRs(const unsigned uUserKey, std::auto_ptr<SRespondMsg> pRs)
{
	if (0 == pRs->uResult)
	{
		LOGM("��¼����Key:%d �ɹ���", uUserKey);
		//if (EST_CMD_SERVER == uUserKey)
		//{
		//	int iUserType = 1;
		//	//int iUserType = m_pMainModel->GetUserInfo().m_iUserType;
		//	emit signalShowMainWindow(iUserType);
		//}
	}
	else
		LOGE("��¼����Key:%d ʧ�ܣ�", uUserKey);
}
