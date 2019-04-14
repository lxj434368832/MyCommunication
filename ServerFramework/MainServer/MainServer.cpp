#include "MainServer.h"
#include "../../IOCPCommunication/IOCPServer.h"
#include "../../IOCPCommunication/INetInterface.h"
#include "../../Framework/include/log/logging.h"
#include "../../Framework/include/cfg_reg_reader.h"
#include "../../Framework/include/Utils.h"
#include "../CommonFile//CommonDefine.h"
#include "../Component/MessageHandle/MessageHandle.h"
#include "../Model/ModelManage/ModelManage.h"
#include "../Component/TCPServer/CommunicationBusiness.h"

MainServer::MainServer()
{
#ifdef ZX_LOGGING_H_
	zxl::zx_logger *logger = zxl::zx_logger::instance();
	zxl::configure::add_flag(zxl::config_flag::rolling_by_day);		//dgx�����ð�ʱ���ӡ��־
	zxl::configure tmp_cfg(zxl::helpers::get_module_dir() + "\\config\\LogConfig.ini");
	logger->reconfiguration(tmp_cfg);
#endif

	m_pMessage = new MessageHandle(this);
	m_pCommunication = new CommunicationBusiness(this);
	m_pModel = new ModelManage(this);
}

MainServer::~MainServer()
{
	RELEASE(m_pCommunication);
	RELEASE(m_pModel);
	RELEASE(m_pMessage);
	zxl::zx_logger::delete_instance();
}

IMessageHandle* MainServer::GetMessageBusiness()
{
	return m_pMessage;
}

IModelManage* MainServer::GetManageBusiness()
{
	return m_pModel;
}

ICommunication * MainServer::GetCommunication()
{
	return m_pCommunication;
}

ServerConfig* MainServer::GetServerConfig()
{
	return &m_srvConfig;
}

bool MainServer::Start()
{
	if (false == ReadConfigFile()) return false;
	if (false == m_pMessage->Start()) return false;
	if (false == m_pModel->Start()) return false;
	if (false == m_pCommunication->Start()) return false;
	return true;
}

void MainServer::Stop()
{
	m_pCommunication->Stop();
	m_pModel->Stop();
	m_pMessage->Stop();
}

bool MainServer::ReadConfigFile()
{
	std::string strConfigPath = Utils::get_module_path(nullptr, "\\config\\ServerConfig.ini");
	cfg_reg_reader cfg_reader;
	if (!cfg_reader.read_from_cfg(strConfigPath))
	{
		loge() << "��ȡ�����ļ�:" << strConfigPath.c_str() << "ʧ�ܣ�";
		return false;
	}
	m_srvConfig.usListenPort	= std::stoi(cfg_reader.get_value_from_cfg("Server", "ListenPort", "0"));
	m_srvConfig.uServerThreadCount = std::stoul(cfg_reader.get_value_from_cfg("Server", "ServerThreadCount", "0"));
	m_srvConfig.uMessageThreadCount = std::stoul(cfg_reader.get_value_from_cfg("Server", "MessageThreadCount", "0"));
	m_srvConfig.uInitAcceptCount = std::stoul(cfg_reader.get_value_from_cfg("Server", "InitAcceptCount", "0"));
	m_srvConfig.uHeartbeatTime = std::stoul(cfg_reader.get_value_from_cfg("Server", "HeartbeatTime", "0"));

	if (false == m_srvConfig.CheckValid())
	{
		loge() << "��ȡ�����ļ������������������ļ���";
		return false;
	}
	return true;
}
