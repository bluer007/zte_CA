#pragma once
#include "Public.h"
#include "Init.h"
#include "UI.h"
#include "IniFile.h"
#include "Reve.h"

#include <iostream>
//int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
int main()
{
	CUI *ui = new CUI();
	ui->InitUI();
	CInit *init = new CInit(ui);
	init->Init();
	
	CIniFile inifile;
	inifile.SetRrcord("E:\\Projects\\zte_CA\\config.ini");

	CRever rever("\\Device\\NPF_{A48EE8D8-20AC-498A-915B-FD02B95AE787}");		// ("Realtek PCIe GBE 系列控制器");				//("Realtek PCIe GBE Family Controller");
	int len = 0;
	u_char* captured = NULL;		// (u_char*)0X12;	//
	system("pause");

	rever.StartReve((const u_char **)&captured, &len);

	printf("\n\n%d\n", captured);
	printf("\n\n开始--\n");
	int num = 0;
	while (num < len)
	{
		printf("%d-", num);
		printf("% 02x  ", captured[num++]);
	}
	std::cout << len<<endl;


	system("pause");
	delete init;
	delete ui;
	return 0;
}