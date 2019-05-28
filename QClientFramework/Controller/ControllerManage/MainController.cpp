#include "MainController.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Model/ModelManage/MainModel.h"
#include "../../Component/MessageHandle/HandleRequestMessage.h"
#include "../../Component/TCPCommunication/ITCPCommunication.h"

MainController::MainController(ControllerManage *pCtrlMng) :
    ControllerColleague(pCtrlMng)
{
	//�����źźͲ�
    connect(m_pTcpCmmnt, SIGNAL(signalTcpConnectNotify(uint,bool)), SLOT(slotTcpConnectNotify(uint,bool)));
    connect(m_pTcpCmmnt,SIGNAL(signalTcpDisconnectNotify(uint)),SLOT(slotTcpDisconnectNotify(uint)));

    moveToThread(&m_thread);
    m_thread.start();
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
