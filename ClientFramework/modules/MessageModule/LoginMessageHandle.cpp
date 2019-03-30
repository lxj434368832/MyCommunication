#include"LoginMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "MessageModule.h"
#include "..\..\include\CommonDefine.h"
#include "../../MainClient/IMainClient.h"
#include "../CommunicationModule/ICommunication.h"
#include "../CommunicationModule/ServerConnect.h"

LoginMessageHandle::LoginMessageHandle(MessageModule *pMsg)
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
	m_pMsgModule->GetMainClient()->GetMainCommunication()->SendData(uUserKey, strMsg.data(), strMsg.length());
}

void LoginMessageHandle::HandleLoginResponse(const unsigned uUserKey, const pbmsg::Message *msg, void* ptr)
{
	logm() << "�յ�������:" << uUserKey << "���͵ĵ�¼��Ӧ��Ϣ��";
	pbmsg::Response loginrq = msg->response();
	if (loginrq.result())
	{
		logm() << "��¼����˳ɹ���";
	}
	else
	{
		loge() << "��¼�����ʧ�ܣ�������ϢΪ:" << loginrq.error_describe();
		m_pMsgModule->GetMainClient()->GetMainCommunication()->GetServerConnect()->Disconnect(uUserKey);
	}
}
