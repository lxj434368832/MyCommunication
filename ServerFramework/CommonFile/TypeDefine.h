#pragma once

#include <string>

typedef unsigned UserKey;
typedef unsigned UserId;

struct SServerConfig
{
	unsigned short  usListenPort = 6666;         // �����˿�
	unsigned int    uServerThreadCount = 4;			// ��ɶ˿ڼ��������߳�����
	unsigned int    uMessageThreadCount = 4;			// ��Ϣ�����߳�����
	unsigned int    uInitAcceptCount = 10;           // ����˳�ʼ���ܸ���
	unsigned int    uHeartbeatTime = 6000;			// ��ʱ�������ʱ��ms

	// ��Ҫ�ж϶�ȡ�����������Ƿ���ȷ
	bool CheckValid();
};

struct SUserInfo
{
	unsigned	uUserKey;
	unsigned	uUserId;
	std::string	strLoginName;
	std::string strPassword;
	std::string	strName;
	std::string strSex;
	unsigned	uAge;
	unsigned	uHeartCount = 0;
	void Reset();
};

//namespace Json {
//	class Value;
//}
namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

struct SPbMsg
{
	unsigned	uMsgType;
	std::string	strMsg;
};

//���ݽ����Ĺ����ṹ
struct SDataExchange
{
	std::string strStructName;//�ṹ������

	bool ParsePbMsg(const char* pData, unsigned uLength, ::google::protobuf::Message *pMsg);
	virtual bool ParseFromPb(const char* pData, unsigned uLength);
	virtual bool ParseFromPbMsg(::google::protobuf::Message *pMsg);

	virtual SPbMsg SerializeAsPbMsg();
	SPbMsg PackPbMsg(unsigned uMsgType, std::string strMsg);
};

struct SRespondMsg : SDataExchange
{
	unsigned	uRsMsgType = 0;
	unsigned	uResult = 0;
	std::string strMsg;

	virtual SPbMsg SerializeAsPbMsg();
};

struct SHeartBeatNt : SDataExchange
{
	unsigned	uUserId = 0;

	bool ParseFromPb(const char* pData, unsigned uLength) override;
};

struct SLoginRq : SDataExchange
{
	unsigned		uUserId = 0;
	std::string		strUserName;
	std::string		strPassword;

	bool ParseFromPb(const char* pData, unsigned uLength) override;
};
