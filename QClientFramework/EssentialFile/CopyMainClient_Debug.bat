@echo off
echo ��� QAmbitionClient

rem ɾ��Ŀ¼
rmdir /s /Q QAmbitionClient

rem ����Ŀ¼
md QAmbitionClient

rem ���UpperComputer
xcopy /y /s /f Debug\QAmbitionClient.exe 					QAmbitionClient\
xcopy /y /s /f ..\EssentialFile\config\ClientConfig.ini 	QAmbitionClient\config\
rem xcopy /y /s /f ..\3rdParty\libcurl\bin\* 						QAmbitionClient\
rem xcopy /y /s /f qt\* 												QAmbitionClient\
D:\Qt\Qt5.6.3\5.6.3\msvc2013\bin\windeployqt.exe 	QAmbitionClient\QAmbitionClient.exe

rem ����PDB�ļ�
xcopy /y /s /f Debug\*.pdb 			QAmbitionClient\PDB\

pause 