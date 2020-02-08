#include "PbMessageFactory.h"
#include "../../Framework/include/LogFile.h"



PbMessageFactory::PbMessageFactory()
{
}


PbMessageFactory::~PbMessageFactory()
{
}

SDataExchange* PbMessageFactory::GetMsgStruct(unsigned eMsgType, const char* data, unsigned length)
{
	SDataExchange *pData = nullptr;

	switch (eMsgType)
	{
	case pbmsg::EHeartbeatNt:
		pData = new SHeartBeatNt;
		break;
	case pbmsg::ELoginRq:
		pData = new SLoginRq;
		break;
	case pbmsg::ELogoutRq:
		break;
	default:
		loge() << "���ܴ������Ϣ����:" << (int)eMsgType;
		break;
	}
	
	if (nullptr == pData) return pData;

	if (false == pData->ParseFromPb(data, length))
	{
		loge() << "����eMsgType:" << eMsgType << " ��protobuf��Ϣʧ�ܣ�";
		delete pData;
		pData = nullptr;
	}
	return pData;
}
