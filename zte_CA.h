#pragma once
#include "Init.h"
#include "Reve.h"
#include "Sender.h"
#include "Filter.h"
#include "UI.h"

class Czte_CA
{
public:
	Czte_CA();
	~Czte_CA();
	int Begin();
	int End();
	int Login();
	int Logoff();
	int GetIP();
	int CheckOnline();

private:
	int GetIniRecord();					//读取配置文件,并显示在界面
	int GetAdapter(int num = 0, char* adapterName=NULL);					//读取网卡信息,并显示在界面. 如果传有效参数进来, 则将对应num的网卡名字赋值给adaptername

	HANDLE m_thread;
	INI_RECORD* iniRecord;

	int m_islogin;
	CUI* m_ui;
	CRever* m_rever;
	CSender* m_sender;
	CFilter* m_filter;
};