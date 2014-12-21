/*
#include "Public.h"
#include "Init.h"
#include "IniFile.h"

CInit::CInit()
{
	this->m_ui = NULL;
	this->iniRecord = new INI_RECORD();		//初始化this->iniRecord结构体
}

CInit::CInit(CUI* ui)
{
	this->m_ui = ui;
	this->iniRecord = new INI_RECORD();		//初始化this->iniRecord结构体
}

CInit::~CInit()
{
	delete this->iniRecord;
}

INI_RECORD* CInit::iniRecord = NULL;

int CInit::Init()
{
	this->GetAdapter();
	this->GetIniRecord();



	

	return TRUE;
}

int CInit::GetIniRecord()
{
	CIniFile inifile;
	char path[50] = { 0 };
	::GetCurrentDirectory(50, path);
	strcat_s(path, 50, "\\config.ini");
	
	inifile.GetRecord(path, this->iniRecord);				//读取配置信息到iniRecord
	//把配置文件的信息反映到界面上
	m_ui->SetTimeout(&this->iniRecord->Timeout);
	m_ui->SetRememberPWD(&this->iniRecord->RememberPWD);
	m_ui->SetAutorun(&this->iniRecord->Autorun);
	m_ui->SetAutologon(&this->iniRecord->Autologon);
	m_ui->SetTry(&this->iniRecord->Try);
	m_ui->Setnetcard(&this->iniRecord->netcard);
	m_ui->SetLastUser(&this->iniRecord->LastUser);

	char temp[20] = {0};
	int i = 0;
	for (int k = 0, j = 0, len = strlen(this->iniRecord->username); k<len;k++)
	{
		//各个账号的分割用一个 | 来隔开
		if (this->iniRecord->username[k]!='|')
		{
			temp[j++] = this->iniRecord->username[k];
		}
		else
		{
			//把各个账号反映到界面上
			m_ui->Addusername(temp);
			//如果找到当前用户账号,则填上当前用户账号
			if ((++i) == this->iniRecord->LastUser)
				m_ui->Setusername(temp);
			_strset_s(temp,20 ,'\0');
			j = 0;
		}
	}
	i = 0;		//复位
	char *sheng, *p = strtok_s(this->iniRecord->userPWD, "|", &sheng);
	while (p != NULL)
	{
		if ((++i)==this->iniRecord->LastUser)
			m_ui->SetuserPWD(p);	
		p = strtok_s(NULL, "|", &sheng);
	}
	//就算匹配不了密码的分隔符|,就直接利用初始化界面的框框为空
	return TRUE;
}

int CInit::GetAdapter()
{
	pcap_if_t *alldevs;		//网卡适配器列表
	pcap_if_t *d;			//临时网卡适配器
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];		//错误信息缓冲区

	/ * 取得网卡列表 * /
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		m_ui->OutError(NO_WINPAP);		//错误：寻找网卡失败！请安装或者重新安装Winpap	
		return FALSE;
	}

	/ * 输出列表 * /
	for (d = alldevs; d; d = d->next)
	{
		i++;
		if (d->description)
			m_ui->AddAdapter(d->description);		//输出网卡信息到界面的网卡列表中
		else
		{
			m_ui->OutError(NO_Description);		////警告：网卡没有一个有效的描述
			//return FALSE;
		}
		if (i == this->iniRecord->netcard)
			m_ui->SetAdapter(d->description);		//如果检索到跟上次网卡一样的,就选中该网卡

	}
	if (i == 0)
	{
		m_ui->OutError(NO_Adapter);			////错误：可能没有网卡，可以试试重装Winpap
		return FALSE;
	}

	/ * 我们不再需要列表了, 释放 * /
	pcap_freealldevs(alldevs);

	m_ui->OutOK(OK_Init);		//初始化成功，成功加载网卡！
	return TRUE;
}*/