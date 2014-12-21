
#include <process.h>
#include "Public.h"
#include "zte_CA.h"
#include "IniFile.h"
//�����������״̬
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

int Czte_CA::GetIniRecord()					//��ȡ�����ļ�,����ʾ�ڽ���
{
	CIniFile inifile;
	char path[100] = { 0 };
	::GetCurrentDirectory(100, path);
	strcat_s(path, 100, "\\config.ini");

	inifile.GetRecord(path, this->iniRecord);				//��ȡ������Ϣ��iniRecord
	//�������ļ�����Ϣ��ӳ��������
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
		//�����˺ŵķָ���һ�� | ������
		if (this->iniRecord->username[k] != '|')
		{
			temp[j++] = this->iniRecord->username[k];
		}
		else
		{
			//�Ѹ����˺ŷ�ӳ��������
			m_ui->Addusername(temp);
			//����ҵ���ǰ�û��˺�,�����ϵ�ǰ�û��˺�
			if ((++i) == this->iniRecord->LastUser)
				m_ui->Setusername(temp);
			_strset_s(temp, 20, '\0');
			j = 0;
		}
	}
	i = 0;		//��λ
	char *sheng, *p = strtok_s(this->iniRecord->userPWD, "|", &sheng);
	while (p != NULL)
	{
		if ((++i) == this->iniRecord->LastUser)
			m_ui->SetuserPWD(p);
		p = strtok_s(NULL, "|", &sheng);
	}
	//����ƥ�䲻������ķָ���|,��ֱ�����ó�ʼ������Ŀ��Ϊ��
	return TRUE;
}

int Czte_CA::GetAdapter(int num /*= 0*/, char* adapterName /*= NULL*/)					
{
	//��ȡ������Ϣ,����ʾ�ڽ���. �������Ч��������, �򽫶�Ӧnum���������ָ�ֵ��adaptername

	pcap_if_t *alldevs;		//�����������б�
	pcap_if_t *d;			//��ʱ����������
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];		//������Ϣ������

	/* ȡ�������б� */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		m_ui->OutError(NO_WINPAP);		//����Ѱ������ʧ�ܣ��밲װ�������°�װWinpap	
		return FALSE;
	}

	/* ����б� */
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
			m_ui->AddAdapter(d->description);		//���������Ϣ������������б���
		else
		{
			m_ui->OutError(NO_Description);		////���棺����û��һ����Ч������
			//return FALSE;
		}
		if (i == this->iniRecord->netcard)
			m_ui->SetAdapter(d->description);		//������������ϴ�����һ����,��ѡ�и�����

	}
	if (i == 0)
	{
		m_ui->OutError(NO_Adapter);			////���󣺿���û������������������װWinpap
		return FALSE;
	}

	/* ���ǲ�����Ҫ�б���, �ͷ� */
	pcap_freealldevs(alldevs);

	m_ui->OutOK(OK_Init);		//��ʼ���ɹ����ɹ�����������
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



	/*CRever rever("\\Device\\NPF_{A48EE8D8-20AC-498A-915B-FD02B95AE787}");		// ("Realtek PCIe GBE ϵ�п�����");				//("Realtek PCIe GBE Family Controller");
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
	//���������ļ�
	CIniFile inifile;
	char path[100] = { 0 };
	::GetCurrentDirectory(100, path);
	strcat_s(path, 100, "\\config.ini");
	inifile.SetRrcord(path);

	//�������߰�
	this->Logoff();

	return TRUE;
}

int Czte_CA::Login()
{
	static int i = 0;
	if (0 == i)
	{
		//���ѡ����Ч��
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
			//����start��ʼ��
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

	//����logoff���߰�
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
	DWORD   flags;//������ʽ
	BOOL   m_bOnline;//�Ƿ�����
	m_bOnline = ::InternetGetConnectedState(&flags, 0);

	if (m_bOnline)//����
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