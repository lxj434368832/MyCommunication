#include "MainLogic.h"



MainLogic::MainLogic(IMainClient *_main = nullptr)
	:ILogicInterface(_main)
{
}


MainLogic::~MainLogic()
{
}

bool MainLogic::Start()
{
	return true;
}

void MainLogic::Stop()
{
}
