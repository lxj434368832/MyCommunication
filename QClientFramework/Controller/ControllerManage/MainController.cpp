#include "MainController.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/MainModel.h"
#include "../../Component/MessageHandle/HandleRequestMessage.h"
#include "../../Component/TCPCommunication/ITCPCommunication.h"
#include "../../Component/Utility/NetworkHelp.h"
#include "../../MainClient/IMainClient.h"

MainController::MainController(ControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	//�����źźͲ�
	connect(this, SIGNAL(signalUserLogin(std::string, std::string)), SLOT(slotUserLogin(std::string, std::string)));
	connect(this, SIGNAL(signalExecuteSystem()), SLOT(slotExecuteSystem()));
    connect(m_pTcpCmmnt, SIGNAL(signalTcpConnectNotify(uint,bool)), SLOT(slotTcpConnectNotify(uint,bool)));
    connect(m_pTcpCmmnt,SIGNAL(signalTcpDisconnectNotify(uint)),SLOT(slotTcpDisconnectNotify(uint)));

	//�����첽�߳�
    moveToThread(&m_thread);
    m_thread.start();


	//ע��ͨѶ��Ϣ����
	//m_pTcpCmmnt->RegistMessageHandle(EMsgType::e_user_login_rs, std::bind(&MainController::HandlTcpLoginRespondMsg,
	//	this, std::placeholders::_1, std::placeholders::_2));

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

bool MainController::Start()
{
    m_pMainModel = m_pModel->GetMainModel();
     if(nullptr == m_pMainModel)
     {
         loge()<< "��ȡ��ģ��ʧ��!";
         return false;
     }

    return true;
}

void MainController::Stop()
{
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
	if (false == m_pTcpCmmnt->ConnectServer())
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
		m_pMainModel->AddUser(uServerType);
		std::string strMsg = m_pHandleRqMsg->BuildLoginRequest("mingqiaowen", "123456");
		m_pTcpCmmnt->SendData(uServerType, strMsg.data(), strMsg.length());
	}
}

void MainController::slotTcpDisconnectNotify(unsigned uServerType)
{
	LOGM("userKey:%d���ӶϿ�",uServerType);
	m_pMainModel->DeleteUser(uServerType);
}

void MainController::UserLoginResult(unsigned uUserKey, bool bRet, std::string strMsg)
{
	LOGM("�û�:%d ��¼�����%2s", uUserKey, strMsg.c_str());
}
