#include "HandleRespondMessage.h"
#include "IMessageHandle.h"
#include "PbMessageHandle.h"
#include "Message.pb.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../Controller/ControllerManage/MainController.h"


HandleRespondMessage::HandleRespondMessage(IMessageHandle *pMsgHandle)
	:HandleMessageColleague(pMsgHandle)
{
	PbMessageHandle *pMsgFctry = pMsgHandle->GetProtobufMsgFactory();
	//pMsgFctry->RegisterMessageFunction(pbmsg::ELoginRs, std::bind(&HandleRespondMessage::HandleLoginRespond,
	//	this, std::placeholders::_1, std::placeholders::_2));
}

HandleRespondMessage::~HandleRespondMessage()
{
}

void HandleRespondMessage::HandleLoginRespond(const unsigned uUserKey, const pbmsg::Message *msg)
{
	if (pbmsg::ELoginRs != msg->msgtype())
	{
		loge() << "消息类型:" << msg->msgtype() << "错误！";
	}

	const pbmsg::Respond &msgRs = msg->respond();
	//调用控制层处理登录信息
	//m_pMainCtrl->UserLoginResult(uUserKey, msgRs.result(), msgRs.errordescribe());
}
