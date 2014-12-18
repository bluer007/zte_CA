#include "Public.h"
#include "zte_CA.h"
#include "IniFile.h"

Czte_CA::Czte_CA()
{

}

Czte_CA::~Czte_CA()
{
	if (this->m_init)
		delete this->m_init;
	if (this->m_rever)
		delete this->m_rever;
	if (this->m_ui)
		delete this->m_ui;
}

int Czte_CA::Begin()
{
	CUI* m_ui = new CUI;
	m_ui->InitUI();
	CInit* m_init = new CInit(m_ui);
	m_init->Init();


	//////////////////////////////////////////////////////////////////////////



	CRever rever("\\Device\\NPF_{A48EE8D8-20AC-498A-915B-FD02B95AE787}");		// ("Realtek PCIe GBE 系列控制器");				//("Realtek PCIe GBE Family Controller");
	int len = 0;
	u_char* captured = NULL;		// (u_char*)0X12;	//
	system("pause");

	rever.StartReve((const u_char **)&captured, &len);
	//////////////////////////////////////////////////////////////////////////


	CRever* m_rever = new CRever;

}

int Czte_CA::End()
{
	//生成配置文件
	CIniFile inifile;
	char path[100] = { 0 };
	::GetCurrentDirectory(100, path);
	strcat_s(path, 100, "\\config.ini");
	inifile.SetRrcord(path);

	//发送下线包
	this->
}