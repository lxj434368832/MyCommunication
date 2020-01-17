#pragma once

#include <queue>
#include <condition_variable>
#include <thread>
#include "../../3rdParty/Framework/include/ResourceManage.h"

namespace pbmsg{ class Message; }
struct SMessageData;

typedef std::function<void(const unsigned uUserKey, const pbmsg::Message *msg)> funMessageHandle;

class ProtobufMsgFactory
{
public:
	ProtobufMsgFactory(void * pMain = nullptr);
	~ProtobufMsgFactory();

	void RegisterMessageFunction(unsigned, funMessageHandle);
	void RemoveMessageFunction(unsigned msgType);

	bool Start(unsigned uThreadCount);
	void Stop();

	//�����������
	void AddMessageData(unsigned uUserKey, const char* data, unsigned length);

private:
	void MessageHandleThread();

private:
	void*						m_pMain;			//���ݸ���Ϣ�����ָ��
	bool						m_bStart;			//�Ƿ�ʼ�ı�ʶ
	std::vector<std::thread*>	m_threadList;		//�̳߳��б�

	mqw::ResourceManage<SMessageData> m_rscMessage;	//��Ϣ��Դ�����б�
	std::queue<SMessageData*>	m_msgList;			//���������Ϣ����
	std::mutex					m_mutexMsgList;
	std::condition_variable		m_cvConsumer;

	std::map<unsigned, funMessageHandle> m_mapMsgHandle;
};

