#include"LoginMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "MessageHandle.h"
#include "..\..\CommonFile\CommonDefine.h"
#include "../../MainClient/IMainClient.h"
#include "../TCPCommunication/ITCPCommunication.h"
#include "../TCPCommunication/ServerConnect.h"

LoginMessageHandle::LoginMessageHandle(MessageHandle *pMsg)
	:IMessageHandle(pMsg)
{
	ProtobufMsgFactory *pMsgFctry = pMsg->GetProtobufMsgFactory();
	pMsgFctry->RegisterMessageFunction(pbmsg::MSG::Login_Response,std::bind(&LoginMessageHandle::HandleLoginResponse,
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));;
}

LoginMessageHandle::~LoginMessageHandle()
{

}

void LoginMessageHandle::SendLoginMessage(unsigned uUserKey)
{
	pbmsg::Message msg;
	msg.set_msg_type(pbmsg::Login_Request);
	msg.set_sequence(0);
	pbmsg::Request *rq = msg.mutable_request();
	pbmsg::LoginRequest *login = rq->mutable_login();
	login->set_username("mingel");
	login->set_password("mingqiaowen");
	
	std::string strMsg = msg.SerializeAsString();
	m_pMsgModule->GetMainClient()->GetTCPCommunication()->SendData(uUserKey, strMsg.data(), strMsg.length());
}

void LoginMessageHandle::HandleLoginResponse(const unsigned uUserKey, const pbmsg::Message *msg, void* ptr)
{
	logm() << "�յ�������:" << uUserKey << "���͵ĵ�¼��Ӧ��Ϣ��";
	pbmsg::Response loginRs = msg->response();
	if (loginRs.result())
	{
		logm() << "��¼����˳ɹ���";
	}
	else
	{
		loge() << QString::fromLocal8Bit("��¼�����ʧ�ܣ�������ϢΪ:%1").arg(loginRs.error_describe().c_str());
		m_pMsgModule->GetMainClient()->GetTCPCommunication()->GetServerConnect()->Disconnect(uUserKey);
	}
}
