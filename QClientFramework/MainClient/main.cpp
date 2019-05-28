#include <vld.h>
#include "MainClient.h"
#include "../CommonFile/CommonDefine.h"
#include "ImportLibrary.h"
#include <string>
#include <QApplication>
//#include <QTextCodec>

//#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    LogFile log;

    IMainClient *pMain = new MainClient;
    if (false == pMain->Start())
    {
        loge() << "������ʧ�ܣ���鿴��־��";
		system("pause");
    }
	else
		a.exec();

    pMain->Stop();
    RELEASE(pMain);
	return 0;
}

