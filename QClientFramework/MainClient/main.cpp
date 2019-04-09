#include <vld.h>
#include "MainClient.h"
#include "../CommonFile/CommonDefine.h"
#include "../3rdParty/MLog/include/MLog.h"
#include "ImportLibrary.h"
#include <string>
#include <QApplication>
//#include <QTextCodec>

//#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    MLog log;

    IMainClient *pMain = new MainClient;
    if (false == pMain->Start())
    {
        loge() << QStringLiteral("������ʧ�ܣ���鿴��־��");
    }

    a.exec();
    pMain->Stop();
    RELEASE(pMain);
	return 0;
}

