#include"HandleRequestMessage.h"
#include "IMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "../../CommonFile/CommonDefine.h"

HandleRequestMessage::HandleRequestMessage(IMessageHandle *pMsgHandle)
	:IMessageColleague(pMsgHandle)
{
	ProtobufMsgFactory *pMsgFctry = pMsgHandle->GetProtobufMsgFactory();
	pMsgFctry->RegisterMessageFunction(pbmsg::MSG::ELoginRequest,std::bind(&HandleRequestMessage::HandleLoginRequest,
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

HandleRequestMessage::~HandleRequestMessage()
{

}

std::string HandleRequestMessage::BuildLoginRequest(std::string strUserName, std::string strPassword)
{
	pbmsg::Message msg;
	msg.set_msgtype(pbmsg::ELoginRequest);
	msg.set_sequence(1);

	pbmsg::Request *msgRq = msg.mutable_request();
	pbmsg::LoginRequest *loginRq = msgRq->mutable_login();
	loginRq->set_username(strUserName);
	loginRq->set_password(strPassword);

	return msg.SerializeAsString();
}

void HandleRequestMessage::HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr)
{
	if (pbmsg::MSG::ELoginRequest != msg.msgtype())
	{
		loge() << "��Ϣ����:" << msg.msgtype() << "����";
	}
	const pbmsg::LoginRequest &login = msg.request().login();
	logm() << " �û�:" << login.username() << "��¼��������:" << login.password();

	//���ÿ��Ʋ㴦���¼��Ϣ

}
