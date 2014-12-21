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
	int GetIniRecord();					//��ȡ�����ļ�,����ʾ�ڽ���
	int GetAdapter(int num = 0, char* adapterName=NULL);					//��ȡ������Ϣ,����ʾ�ڽ���. �������Ч��������, �򽫶�Ӧnum���������ָ�ֵ��adaptername

	HANDLE m_thread;
	INI_RECORD* iniRecord;

	int m_islogin;
	CUI* m_ui;
	CRever* m_rever;
	CSender* m_sender;
	CFilter* m_filter;
};