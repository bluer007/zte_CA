/*
#pragma once

#include "Public.h"
#include "UI.h"

class CInit
{
public:
	CInit();
	CInit(CUI* ui);
	~CInit();
	int Init();
	static 	INI_RECORD* iniRecord;

private:
	int GetIniRecord();					//读取配置文件,并显示在界面
	int GetAdapter();					//读取网卡信息,并显示在界面
	CUI *m_ui;

	
};*/