#include"HandleRequestMessage.h"
#include "IMessageHandle.h"
#include "PbMessageHandle.h"
#include "Message.pb.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/TypeDefine.h"
#include "../../Controller/ControllerManage/MainController.h"

HandleRequestMessage::HandleRequestMessage(IMessageHandle *pMsgHandle)
	:HandleMessageColleague(pMsgHandle)
{
	PbMessageHandle *pMsgFctry = pMsgHandle->GetPbMessageHandle();
	//pMsgFctry->RegisterMessageFunction(pbmsg::EMsg::ELoginRq,std::bind(&HandleRequestMessage::HandleLoginRequest,
	//	this, std::placeholders::_1, std::placeholders::_2));
}

HandleRequestMessage::~HandleRequestMessage()
{

}

void HandleRequestMessage::HandleLoginRequest(const unsigned uUserKey, const pbmsg::Message *msg)
{
	if (pbmsg::EMsg::ELoginRq != msg->msgtype())
	{
		loge() << "��Ϣ����:" << msg->msgtype() << "����";
	}
	const pbmsg::LoginRq &login = msg->request().login();

	SUserInfo	userInfo;
	userInfo.uUserKey = uUserKey;
	userInfo.strName = login.username();
	userInfo.strPassword = login.password();

	//���ÿ��Ʋ㴦���¼��Ϣ
	//m_pMainCtrl->UserLogin(userInfo);
}
