#pragma once
/*************************************************************************
* function����ʱ���Զ���
* author :	������
* datetime��2018-01-05
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include <wtypes.h>
#include <map>
#include <mutex>
#include <functional>

typedef std::function<void(DWORD timerID)> funTimerProc;

class MTimer
{
public:
	MTimer();
	~MTimer();
	DWORD GetTimerId();

	/*************************************************************************
	* function��		����һ����ʱ������ʱ������ÿ�δ������֮�������¶�ʱ
	* param uElapse:	��ʱʱ��
	* param lpTimerFunc:�ص�����
	* param isStartCall:�Ƿ�һ�����͵���
	* return bool:		�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	bool start(UINT uElapse, funTimerProc lpTimerFunc,bool isStartCall=false);

	/*************************************************************************
	* function��		ֹͣǰ�洴���Ķ�ʱ��
	* return void:		��
	*************************************************************************/
	void stop();

	/*************************************************************************
	* function��		����һ����ʱ�����ߵ���SetTimer������
	* param uElapse:	��ʱʱ��
	* param lpTimerFunc:�ص�����
	* param dwUser:		�û��Զ�������
	* return bool:		�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	bool start_timer(UINT uElapse, funTimerProc lpTimerFunc);

	void stop_timer();
	
private:
	static DWORD WINAPI onThreadProc(LPVOID lpParameter);
	DWORD ThreadProc();

	static DWORD WINAPI onThreadProc1(LPVOID lpParameter);
	DWORD ThreadProc1();

	static void CALLBACK onTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

private:
	HANDLE			m_hThread = NULL;
	bool			m_bStart = true;
	UINT			m_uElapse = 0;
	funTimerProc	m_funTimerProc;
	bool			m_bIsStartCall = false;

	DWORD_PTR		m_dwUser = 0;

	UINT			m_uTimerID = 0;
	funTimerProc	m_funTimerProc1;
	static	std::map<UINT, funTimerProc>	s_mapUser;	//ʱ��id���û����ݵ�ӳ���ϵ
	static std::recursive_mutex			s_mtxMap;	//
};

