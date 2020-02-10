#include "MessageHandle.h"
#include "../../MainClient/IMainClient.h"
#include "../../Controller/ControllerManage/IControllerManage.h"
#include "../../Controller/ControllerManage/MainController.h"
#include "PbMessageHandle.h"
#include "HandleRequestMessage.h"
#include "HandleRespondMessage.h"
#include "HandleNotifyMessage.h"
#include "../../CommonFile/CommonDefine.h"
#include "../../CommonFile/TypeDefine.h"
#include <../../3rdParty/Protobuf/include/Message.pb.h>


MessageHandle::MessageHandle(IMainClient *pMain)
	:IMessageHandle(pMain)
{
	m_pPbMessageHandle = new PbMessageHandle;

	m_pHandleRqMsg = new HandleRequestMessage(this);
	m_pHandleRsMsg = new HandleRespondMessage(this);
	m_pHandleNtMsg = new HandleNotifyMessage(this);
}

MessageHandle::~MessageHandle()
{
	RELEASE(m_pHandleNtMsg);
	RELEASE(m_pHandleRsMsg);
	RELEASE(m_pHandleRqMsg);
	RELEASE(m_pPbMessageHandle);
}

HandleRequestMessage * MessageHandle::GetHandleRequestMessage()
{
	return m_pHandleRqMsg;
}

HandleRespondMessage * MessageHandle::GetHandleRespondMessage()
{
	return m_pHandleRsMsg;
}

HandleNotifyMessage * MessageHandle::GetHandleNotifyMessage()
{
	return m_pHandleNtMsg;
}

PbMessageHandle* MessageHandle::GetProtobufMsgFactory()
{
	return m_pPbMessageHandle;
}

bool MessageHandle::Initialize()
{
	logm() << "************��ʼ��Ϣ����ĳ�ʼ��************";
	if (false == m_pPbMessageHandle->Initialize(
		m_pMain->GetClientConfig()->uMessageThreadCount)) return false;
	if (false == m_pHandleRqMsg->Initialize()) return false;
	if (false == m_pHandleNtMsg->Initialize())	return false;
	if (false == m_pHandleRsMsg->Initialize())	return false;
	return true;
}

void MessageHandle::Uninitialize()
{
	m_pPbMessageHandle->Uninitialize();
	m_pHandleRqMsg->Uninitialize();
	m_pHandleRsMsg->Uninitialize();
	m_pHandleNtMsg->Uninitialize();
}

void MessageHandle::RegisterMessageHandle()
{
	/*************************ע��MainController����Ϣ����*************************/
	MainController *pMainCtrl = m_pMain->GetControllerManage()->GetMainController();
	m_pPbMessageHandle->RegisterMessageFunction(pbmsg::ELoginRs,
		std::bind(&MainController::HandleLoginRs, pMainCtrl, std::placeholders::_1, std::placeholders::_2));


	/*************************ע��Controller����Ϣ����*************************/

}

void MessageHandle::HandleProtobufMessage(unsigned uUserKey, unsigned uMsgType, const char * data, unsigned length)
{
	m_pPbMessageHandle->AddMessageData(uUserKey, uMsgType, data, length);
}
