#pragma once

#include <queue>
#include <condition_variable>
#include "Message.pb.h"
#include "../../Framework/include/ResourceManage.h"

typedef std::function<void(const unsigned uUserKey, const pbmsg::Message &msg, void* ptr)> funMessageHandle;

class ProtobufMsgFactory
{
public:
	ProtobufMsgFactory(void * srv = nullptr);
	~ProtobufMsgFactory();

	void RegisterMessageFunction(pbmsg::MSG, funMessageHandle);
	void RemoveMessageFunction(pbmsg::MSG msgType);

	bool Start(unsigned uThreadCount);
	void Stop();

	//�����������
	void AddMessageData(unsigned uUserKey, const char* data, unsigned length);

private:
	void MessageHandleThread();

private:
	struct SMessageData 
	{
		unsigned		m_uUserKey;
		pbmsg::Message	m_msg;
	};
	void*						m_pSrv;				//���ݸ���Ϣ�����ָ��
	bool						m_bStart;			//�Ƿ�ʼ�ı�ʶ
	std::vector<std::thread*>	m_threadList;		//�̳߳��б�

	mqw::ResourceManage<SMessageData> m_rscMessage;	//��Ϣ��Դ�����б�
	std::queue<SMessageData*>	m_msgList;			//���������Ϣ����
	std::mutex					m_mutexMsgList;
	std::condition_variable		m_cvConsumer;

	std::map<pbmsg::MSG, funMessageHandle> m_mapMsgHandle;
};

