#include "TypeDefine.h"
#include "CommonDefine.h"
#include <Message.pb.h>

void SUserInfo::Reset()
{
	uUserKey = 0;
	uUserId = 0;
	strName.clear();
	strPassword.clear();
	strSex.clear();
}

bool SServerConfig::CheckValid()
{
	if (usListenPort == 0
		|| uInitAcceptCount == 0
		|| uServerThreadCount == 0
		|| uHeartbeatTime == 0)
	{
		return false;
	}
	return true;
}

bool SDataExchange::ParsePbMsg(const char* pData, unsigned uLength, ::google::protobuf::Message *pMsg)
{
	bool bSuc = false;
	try
	{
		if (pMsg->ParseFromArray(pData, uLength))
		{
			bSuc = true;
		}
		else
		{
			loge() << "����" << strStructName << "ʧ�ܣ�data:" << pData;
		}
	}
	catch (std::exception* e)
	{
		loge() << "����" << strStructName << "�쳣:" << e->what() << "\ndata:" << pData;
	}
	return bSuc;
}

bool SDataExchange::ParseFromPb(const char* pData, unsigned uLength)
{
	strStructName = "protobuf ��Ϣ";
	//::google::protobuf::Message msg;
	//if (false == ParsePbMsg(pData, uLength, &msg)) return false;
	//to do msg assign

	return true;
}

bool SDataExchange::ParseFromPbMsg(::google::protobuf::Message *pMsg)
{
	strStructName = "protobuf ��Ϣ";
	//to do param assign

	return true;
}

SPbMsg SDataExchange::SerializeAsPbMsg()
{
	pbmsg::ClientMsg msg;
	msg.set_msg_type(pbmsg::ELoginRq);
	msg.set_msg_data("�ͻ�����Ϣ");
	return PackPbMsg(pbmsg::ELoginRq, msg.SerializeAsString());
}

SPbMsg SDataExchange::PackPbMsg(unsigned uMsgType, std::string strMsg)
{
	SPbMsg pbMsg;
	pbMsg.uMsgType = uMsgType;
	pbMsg.strMsg = strMsg;
	return pbMsg;
}

SPbMsg SRespondMsg::SerializeAsPbMsg()
{
	pbmsg::RespondMsg msg;
	msg.set_result(0);
	msg.set_result_msg(strMsg);
	return PackPbMsg(uRsMsgType, msg.SerializeAsString());
}

bool SHeartBeatNt::ParseFromPb(const char* pData, unsigned uLength)
{
	strStructName = "������Ϣ";
	pbmsg::LoginRq rq;
	if (false == ParsePbMsg(pData, uLength, &rq)) return false;
	uUserId = rq.user_id();
	return true;
}

bool SLoginRq::ParseFromPb(const char* pData, unsigned uLength)
{
	strStructName = "��¼��Ϣ";
	pbmsg::LoginRq rq;
	if (false == ParsePbMsg(pData, uLength, &rq)) return false;
	uUserId = rq.user_id();
	strUserName = rq.username();
	strPassword = rq.password();
	return true;
}
