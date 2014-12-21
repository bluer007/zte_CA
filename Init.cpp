/*
#include "Public.h"
#include "Init.h"
#include "IniFile.h"

CInit::CInit()
{
	this->m_ui = NULL;
	this->iniRecord = new INI_RECORD();		//��ʼ��this->iniRecord�ṹ��
}

CInit::CInit(CUI* ui)
{
	this->m_ui = ui;
	this->iniRecord = new INI_RECORD();		//��ʼ��this->iniRecord�ṹ��
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
	
	inifile.GetRecord(path, this->iniRecord);				//��ȡ������Ϣ��iniRecord
	//�������ļ�����Ϣ��ӳ��������
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
		//�����˺ŵķָ���һ�� | ������
		if (this->iniRecord->username[k]!='|')
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
			_strset_s(temp,20 ,'\0');
			j = 0;
		}
	}
	i = 0;		//��λ
	char *sheng, *p = strtok_s(this->iniRecord->userPWD, "|", &sheng);
	while (p != NULL)
	{
		if ((++i)==this->iniRecord->LastUser)
			m_ui->SetuserPWD(p);	
		p = strtok_s(NULL, "|", &sheng);
	}
	//����ƥ�䲻������ķָ���|,��ֱ�����ó�ʼ������Ŀ��Ϊ��
	return TRUE;
}

int CInit::GetAdapter()
{
	pcap_if_t *alldevs;		//�����������б�
	pcap_if_t *d;			//��ʱ����������
	int i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];		//������Ϣ������

	/ * ȡ�������б� * /
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		m_ui->OutError(NO_WINPAP);		//����Ѱ������ʧ�ܣ��밲װ�������°�װWinpap	
		return FALSE;
	}

	/ * ����б� * /
	for (d = alldevs; d; d = d->next)
	{
		i++;
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

	/ * ���ǲ�����Ҫ�б���, �ͷ� * /
	pcap_freealldevs(alldevs);

	m_ui->OutOK(OK_Init);		//��ʼ���ɹ����ɹ�����������
	return TRUE;
}*/