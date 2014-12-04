#pragma once
#include "Public.h"
#include "Init.h"
#include "UI.h"
#include "IniFile.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CUI *ui = new CUI();
	ui->InitUI();
	CInit *init = new CInit(ui);
	init->Init();
	
	//CIniFile inifile;
	//inifile.SetRrcord("E:\\Projects\\zte_CA\\config.ini");
	system("pause");
	delete init;
	delete ui;
	return 0;
}