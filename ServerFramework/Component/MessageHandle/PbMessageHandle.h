#pragma once

#include <queue>
#include <condition_variable>
#include <thread>
#include "../../3rdParty/Framework/include/ResourceManage.h"

namespace pbmsg{ class Message; }
struct SMessageData;
struct SDataExchange;

typedef std::function<void(const unsigned uUserKey, SDataExchange*)> funMessageHandle;

class PbMessageHandle
{
public:
	PbMessageHandle(void * pMain = nullptr);
	~PbMessageHandle();

	/*************************************************************************
	* function:				ע��/�Ƴ���Ϣ������
	* param unsigned:		��Ϣ����
	* param funMessageHandle: ��ǰ��Ϣ��Ӧ�Ĵ�������Ϊ0��ʾ�Ƴ�
	* return:
	*************************************************************************/
	void RegisterMessageFunction(unsigned, funMessageHandle);

	bool Initialize(unsigned uThreadCount);
	void Uninitialize();

	//�����������
	void AddMessageData(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned length);

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
