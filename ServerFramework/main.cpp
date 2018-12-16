// ServerFramework.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "MainServer.h"
#include "..\IOCPCommunication\stdafx.h"

#define SZAPPNAME "ServerFramework"
int main()
{
	IMainServer *pMain = new MainServer;
	do 
	{
		char strCmd[256];
		//printf("%s -install /-i          to install the service\n", SZAPPNAME);
		//printf("%s -remove /-r           to remove the service\n", SZAPPNAME);
		//printf("%s -debug /-d <params>   to run as a console app for debugging\n", SZAPPNAME);
		printf("%s -start /-s            to startup the service\n", SZAPPNAME);
		printf("%s -stop /-p             to stop the service\n", SZAPPNAME);
		printf("%s -quit /-q             to stop the service\n", SZAPPNAME);

		printf("This may take several seconds,  Please wait.\n");
		
		scanf_s("%s", strCmd, 256);
		if (_stricmp("-start", strCmd) == 0 || _stricmp("-s", strCmd) == 0)
		{
			if (false == pMain->Start())
			{
				loge() << "������ʧ�ܣ���鿴��־��";
			}
		}
		else if (_stricmp("-stop", strCmd) == 0 || _stricmp("-p", strCmd) == 0)
		{
			pMain->Stop();
		}
		else if (_stricmp("-quit", strCmd) == 0 || _stricmp("-q", strCmd) == 0)
		{
			break;
		}

	} while (true);

    return 0;
}

