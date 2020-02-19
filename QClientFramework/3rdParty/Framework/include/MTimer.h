#pragma once
/*************************************************************************
* function���̶߳�ʱ���Զ���
* author :	������
* datetime��2018-01-05
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include <thread>
#include <functional>

typedef std::function<void()> funTimerProc;

class MTimer
{
public:
	MTimer();
	~MTimer();
	/*************************************************************************
	* function��		����һ����ʱ������ʱ������ÿ�δ������֮�������¶�ʱ
	* param uInterval:	��ʱʱ��
	* param lpTimerFunc:�ص�����
	* param isStartCall:�Ƿ�һ�����͵���
	* return bool:		�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	bool Start(unsigned uInterval, funTimerProc lpTimerFunc, bool bStartCall = false);

	/*************************************************************************
	* function��		ֹͣǰ�洴���Ķ�ʱ��
	* return void:		��
	*************************************************************************/
	void Stop();

private:
	void TimerCallback(unsigned uInterval, bool bStartCall);

private:
	std::thread		*m_pTimerThread = nullptr;	//��ʱ���߳�
	HANDLE 			m_hTimerEvent = nullptr;	//��ʱ���¼�
	funTimerProc	m_funTimerProc = nullptr;	//�ص�����
};

