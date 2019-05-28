#include "ProtobufMsgFactory.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/EnumDefine.h"
#include "Message.pb.h"

struct SMessageData
{
	unsigned		m_uUserKey;
	pbmsg::Message	m_msg;
};


ProtobufMsgFactory::ProtobufMsgFactory(void * pMain)
	:m_rscMessage(MESSAGE_RESOURCE_COUNT)
{
	m_pMain = pMain;
	m_bStart = false;
}

ProtobufMsgFactory::~ProtobufMsgFactory()
{
	google::protobuf::ShutdownProtobufLibrary();
}

void ProtobufMsgFactory::RegisterMessageFunction(unsigned msgType, funMessageHandle handle)
{
	m_mapMsgHandle[msgType] = handle;
}

void ProtobufMsgFactory::RemoveMessageFunction(unsigned msgType)
{
	auto iter = m_mapMsgHandle.find(msgType);
	if (iter != m_mapMsgHandle.end())
	{
		m_mapMsgHandle.erase(iter);
	}
}

bool ProtobufMsgFactory::Start(unsigned uThreadCount)
{
	if (uThreadCount == 0)
		uThreadCount = 2;

	m_bStart = true;

	// �����û����ݵ��߳�
	for (unsigned i = 0; i < uThreadCount; i++)
	{
		m_threadList.push_back(new std::thread(&ProtobufMsgFactory::MessageHandleThread, this));
	}

	return true;
}

void ProtobufMsgFactory::Stop()
{
	m_pMain = nullptr;
	if (false == m_bStart) return;
	m_bStart = false;

	m_cvConsumer.notify_all();
	for (auto value : m_threadList)
	{
		if(value->joinable())
			value->join();
		delete value;
	}
	m_threadList.clear();

	m_mapMsgHandle.clear();
}

void ProtobufMsgFactory::AddMessageData(unsigned uUserKey, const char* data, unsigned length)
{
	if (false == m_bStart) return;
	SMessageData *msgData = m_rscMessage.get();
	msgData->m_uUserKey = uUserKey;
	if (false == msgData->m_msg.ParseFromArray(data, length))
	{
		m_rscMessage.put(msgData);//��Ϣ����ʧ�ܣ�ֱ�ӷ���
		return;
	}

	m_mutexMsgList.lock();
	m_msgList.push(msgData);
	m_cvConsumer.notify_one();
	m_mutexMsgList.unlock();
}

void ProtobufMsgFactory::MessageHandleThread()
{
	logm() << "MessageHandleThread start.ThreadID:" << std::this_thread::get_id();
	while (m_bStart)
	{
		SMessageData *msgData = nullptr;
		{
			std::unique_lock<std::mutex> lck(m_mutexMsgList);
			while (m_msgList.empty() && m_bStart)
			{
				m_cvConsumer.wait(lck);
			}
			if (false == m_bStart) break;
			msgData = m_msgList.front();
			m_msgList.pop();
		}

		auto it = m_mapMsgHandle.find(msgData->m_msg.msgtype());
		if (it != m_mapMsgHandle.end())
		{
			(it->second)(msgData->m_uUserKey, &msgData->m_msg);
		}
		else
		{
			LOGE("��Ϣ:%dû�ж�Ӧ�Ĵ�������", msgData->m_msg.msgtype());
		}

		m_rscMessage.put(msgData);
	}

	logm() << "ThreadID:" << std::this_thread::get_id()<< "�˳�";
}
