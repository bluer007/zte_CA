
#include <process.h>
#include "Public.h"
#include "zte_CA.h"
#include "IniFile.h"
//检查网络在线状态
#include "Wininet.h"
#pragma comment(lib,"Wininet.lib")


Czte_CA::Czte_CA()
{
	this->m_islogin = FALSE;
	this->iniRecord = new INI_RECORD{0};
}

Czte_CA::~Czte_CA()
{
	if (this->m_rever)
		delete this->m_rever;
	if (this->m_ui)
		delete this->m_ui;
	if (this->iniRecord)
		delete this->iniRecord;
	if (this->m_sender)
		delete this->m_sender;
	
		
}

int Czte_CA::GetIniRecord()					//读取配置文件,并显示在界面
{
	CIniFile inifile;
	char path[100] = { 0 };
	::GetCurrentDirectory(100, path);
	strcat_s(path, 100, "\\config.ini");

	inifile.GetRecord(path, this->iniRecord);				//读取配置信息到iniRecord
	//把配置文件的信息反映到界面上
	m_ui->SetTimeout(&this->iniRecord->Timeout);
	m_ui->SetRememberPWD(&this->iniRecord->RememberPWD);
	m_ui->SetAutorun(&this->iniRecord->Autorun);
	m_ui->SetAutologon(&this->iniRecord->Autologon);
	m_ui->SetTry(&this->iniRecord->Try);
	m_ui->Setnetcard(&this->iniRecord->netcard);
	m_ui->SetLastUser(&this->iniRecord->LastUser);

	char temp[20] = { 0 };
	int i = 0;
	for (int k = 0, j = 0, len = strlen(this->iniRecord->username); k < len; k++)
	{
		//各个账号的分割用一个 | 来隔开
		if (this->iniRecord->username[k] != '|')
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
			_strset_s(temp, 20, '\0');
			j = 0;
		}
	}
	i = 0;		//复位
	char *sheng, *p = strtok_s(this->iniRecord->userPWD, "|", &sheng);
	while (p != NULL)
	{
		if ((++i) == this->iniRecord->LastUser)
			m_ui->SetuserPWD(p);
		p = strtok_s(NULL, "|", &sheng);
	}
	//就算匹配不了密码的分隔符|,就直接利用初始化界面的框框为空
	return TRUE;
}

int Czte_CA::GetAdapter(int num /*= 0*/, char* adapterName /*= NULL*/)					
{
	//读取网卡信息,并显示在界面. 如果传有效参数进来, 则将对应num的网卡名字赋值给adaptername

	pcap_if_t *alldevs;		//网卡适配器列表
	pcap_if_t *d;			//临时网卡适配器
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];		//错误信息缓冲区

	/* 取得网卡列表 */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		m_ui->OutError(NO_WINPAP);		//错误：寻找网卡失败！请安装或者重新安装Winpap	
		return FALSE;
	}

	/* 输出列表 */
	for (d = alldevs; d; d = d->next)
	{
		i++;
		if (num > 0 && NULL != adapterName)
		{
			if (i == num)
			{
				strcpy_s(adapterName, strlen(d->name) + 1, d->name);
				return TRUE;
			}
		}
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

	/* 我们不再需要列表了, 释放 */
	pcap_freealldevs(alldevs);

	m_ui->OutOK(OK_Init);		//初始化成功，成功加载网卡！
	return TRUE;
}


int Czte_CA::Begin()
{
	//CUI* m_ui = new CUI;
	//m_ui->InitUI();
	system("pause");
	this->GetAdapter();
	this->GetIniRecord();


	//////////////////////////////////////////////////////////////////////////



	/*CRever rever("\\Device\\NPF_{A48EE8D8-20AC-498A-915B-FD02B95AE787}");		// ("Realtek PCIe GBE 系列控制器");				//("Realtek PCIe GBE Family Controller");
	int len = 0;
	u_char* captured = NULL;		// (u_char*)0X12;	//
	
	rever.StartReve((const u_char **)&captured, &len);

	system("pause");*/
	//////////////////////////////////////////////////////////////////////////


	//CRever* m_rever = new CRever;
	return TRUE;
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
	this->Logoff();

	return TRUE;
}

int Czte_CA::Login()
{
	static int i = 0;
	if (0 == i)
	{
		//检查选择有效性
		if (0 == this->m_ui->Getnetcard())
		{
			this->m_ui->OutError(NO_Selecte_Adapter);
			return FALSE;
		}
		if ("" == this->m_ui->Getusername())
		{
			this->m_ui->OutError(NO_Username);
			return FALSE;
		}
		if ("" == this->m_ui->GetuserPWD())
		{
			this->m_ui->OutError(NO_Pwd);
			return FALSE;
		}

		i = 1;
		typedef int(_stdcall Czte_CA::*Login) ();
		typedef unsigned(_stdcall * ThreadAddress) (void *);
		Login pThread = (Login)&Czte_CA::Login;
		this->m_thread = (HANDLE)::_beginthreadex(NULL, 0, *(ThreadAddress*)(&pThread), this, 0, NULL);
		
		this->m_islogin = TRUE;
		return TRUE;
	}
	else
	{
		if (this->m_islogin)
		{
			//发送start开始包
			char adapterName[80] = { 0 };
			this->GetAdapter(this->m_ui->Getnetcard(), adapterName);
			this->m_sender = new CSender(adapterName);
			u_char* captured = NULL;
			this->m_rever = new CRever(adapterName);
			this->m_sender->StartSend(RES_START, captured);
			int len = 0, num = 0;
			while (num < 20)
			{
				this->m_rever->StartReve((const u_char **)&captured, &len);
				switch (this->m_filter->StartFilter(captured))
				{
				case REQ_ID:
					this->m_sender->StartSend(RES_ID, captured);
					printf("res_id\n");
					break;
				case REQ_MD5:
					this->m_sender->StartSend(RES_MD5, captured);
					printf("res_md5\n");
					break;
				case REQ_SUCCESS:
					this->GetIP();
					if (this->CheckOnline())
						printf("-------------------success\n");
					break;
				case REQ_KEY:
					this->m_sender->StartSend(RES_KEY1, captured);
					printf("res_key\n");
					num++;
					if (num >= 7)
						this->m_sender->StartSend(RES_LOGOFF, NULL);
					break;
				case REQ_FAILURE:
					printf("-------------------failure\n");
					this->Login();
					num = 20;
					break;
				case UN_KNOWN:
					printf("-------------------unknow\n");
					this->m_sender->StartSend(RES_LOGOFF, NULL);
					break;
				}
			}
		}
		
		system("pause");
		
		i = 0;
		_endthreadex(0);
		return TRUE;

	}

}

int Czte_CA::Logoff()
{
	this->m_islogin = FALSE;

	//发送logoff下线包
	char adapterName[80] = { 0 };
	this->GetAdapter(this->m_ui->Getnetcard(), adapterName);
	CSender sender(adapterName);
	sender.StartSend(RES_LOGOFF, NULL);
	::WaitForSingleObject(this->m_thread, 700);
	if (this->m_thread)
	{
		::CloseHandle(this->m_thread);
		this->m_thread = NULL;
	}

	return TRUE;
}

int Czte_CA::GetIP()
{
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	TCHAR tempBuffer[100];
	char strSystemDirectory[100];
	GetSystemDirectory(tempBuffer, 100);
	sprintf_s(strSystemDirectory, 100, _T("%s\\ipconfig.exe"), tempBuffer);
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.lpFile = strSystemDirectory;
	ShExecInfo.lpParameters = _T("/renew");
	ShExecInfo.nShow = SW_HIDE;
	ShellExecuteEx(&ShExecInfo);
	int res = ::WaitForSingleObject(ShExecInfo.hProcess, this->iniRecord->Timeout * 1000);
	if (WAIT_TIMEOUT == res)
	{
		this->m_ui->OutError(NO_IP);
		return FALSE;
	}
	else if(WAIT_OBJECT_0 == res)
	{
		this->m_ui->OutOK(OK_IP);
		return TRUE;
	}

	return FALSE;
	
}

int Czte_CA::CheckOnline()
{
	DWORD   flags;//上网方式
	BOOL   m_bOnline;//是否在线
	m_bOnline = ::InternetGetConnectedState(&flags, 0);

	if (m_bOnline)//在线
	{
		this->m_ui->OutOK(OK_Online);
		return TRUE;
	}
	else
	{
		if (this->m_islogin)
		{
			this->Logoff();
			this->Login();
		}
		this->m_ui->OutError(NO_Online);
		return FALSE;
	}

}