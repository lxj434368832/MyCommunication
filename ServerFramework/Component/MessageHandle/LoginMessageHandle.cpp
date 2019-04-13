#include"LoginMessageHandle.h"
#include "ProtobufMsgFactory.h"
#include "MessageBusiness.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../MainServer/IMainServer.h"
#include "../../Model/ModelManage/IModelManage.h"
#include "../../Model/ModelManage/UserInfoManage.h"

LoginMessageHandle::LoginMessageHandle(MessageBusiness *pBusiness)
	:IMessageHandle(pBusiness)
{
	ProtobufMsgFactory *pMsgFctry = pBusiness->GetProtobufMsgFactory();
	pMsgFctry->RegisterMessageFunction(pbmsg::MSG::Login_Request,std::bind(&LoginMessageHandle::HandleLoginRequest,
		this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));;
}

LoginMessageHandle::~LoginMessageHandle()
{

}

void LoginMessageHandle::HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr)
{
	if (pbmsg::MSG::Login_Request != msg.msg_type())
	{
		loge() << "��Ϣ����:" << msg.msg_type() << "����";
	}
	const pbmsg::LoginRequest &login = msg.request().login();
	logm() << " �û�:" << login.username() << "��¼��������:" << login.password();

	pbmsg::Message msgRq;
	msgRq.set_msg_type(pbmsg::Login_Response);
	msgRq.set_sequence(1);

	pbmsg::Response *rq = msgRq.mutable_response();
	rq->set_result(true);
	rq->set_last_response(true);
	pbmsg::LoginResponse *loginrq = rq->mutable_login();
	loginrq->set_ttl(1);
	std::string strData = msgRq.SerializeAsString();

	m_pMsgBusiness->GetMainServer()->GetManageBusiness()->GetUserInfoManage()->SendData(uUserKey, strData.data(), strData.length());
}
