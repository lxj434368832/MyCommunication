@echo off
echo ��� WPOSystem

rem ɾ��Ŀ¼
rmdir /s /Q WPOSystem

rem ����Ŀ¼
md WPOSystem

rem ���UpperComputer
xcopy /y /s /f config\UpperComputerConfig.ini 	WPOSystem\WPOUpperComputer\config\
xcopy /y /s /f Release\WPOUpperComputer.exe 	WPOSystem\WPOUpperComputer\
xcopy /y /s /f ..\3rdParty\libcurl\bin\* 					WPOSystem\WPOUpperComputer\
xcopy /y /s /f ..\3rdParty\Qwt\bin\qwt.dll 				WPOSystem\WPOUpperComputer\
xcopy /y /s /f qt\* 												WPOSystem\WPOUpperComputer\
rem ����������д��ؼ�
xcopy /y /s /f hw\*.ocx 			WPOSystem\WPOUpperComputer\
xcopy /y /s /f Release\WPOzHW.exe 	WPOSystem\WPOUpperComputer\
rem D:\Qt\Qt5.6.3\5.6.3\msvc2013\bin\windeployqt.exe 	WPOSystem\WPOUpperComputer\WPOUpperComputer.exe

rem ���Coordinator
xcopy /y /s /f config\CoordinatorConfig.ini 		WPOSystem\WPOCoordinator\config\
xcopy /y /s /f Release\WPOCoordinator.exe 		WPOSystem\WPOCoordinator\
xcopy /y /s /f qt\* 											WPOSystem\WPOCoordinator\

rem ����PDB�ļ�
xcopy /y /s /f Release\*.pdb 			WPOSystem\PDB\



pause 