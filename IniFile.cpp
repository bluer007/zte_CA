#pragma once
#include "Public.h"
#include "IniFile.h"




CIniFile::CIniFile()
{
	this->m_record = new INI_RECORD();		//��ʼ��m_record
}

CIniFile::~CIniFile()
{
	delete this->m_record;
}

int CIniFile::GetRecord(char *path, INI_RECORD *record)
{
	//����ҵ������ļ����ȡ����ļ�¼,�Ҳ������ȡ������Ӧ������Ĭ�ϵ�������Ϣ
	int len = 100;
	char buff[100] = { 0 };
	::GetPrivateProfileString(_T("Config"), _T("Timeout"), _T("200"), buff, len, path);		//��ʱ����,��������2014.11�ղ�����180��,�ֱ�����Ϊ200��
	sscanf_s(buff, "%d", &(this->m_record->Timeout));
	sscanf_s(buff, "%d", &(record->Timeout));

	::GetPrivateProfileString(_T("Config"), _T("RememberPWD"), _T("true"), buff, len, path);	//�Ƿ��ס����,Ĭ�� true
	if (0 == strcmp(buff, "true"))
	{
		this->m_record->RememberPWD = true;
		record->RememberPWD = true;
	}
	else
	{
		this->m_record->RememberPWD = false;
		record->RememberPWD = false;
	}

	::GetPrivateProfileString(_T("Config"), _T("Autorun"), _T("false"), buff, len, path);		//�Ƿ񿪻�����,Ĭ��Ϊ false
	if (0 == strcmp(buff, "true"))
	{
		this->m_record->Autorun = true;
		record->Autorun = true;
	}
	else
	{
		this->m_record->Autorun = false;
		record->Autorun = false;
	}

	::GetPrivateProfileString(_T("Config"), _T("Autologon"), _T("false"), buff, len, path);		//�Ƿ��Զ���¼,Ĭ��Ϊfalse
	if (0 == strcmp(buff, "true"))
	{
		this->m_record->Autologon = true;
		record->Autologon = true;
	}
	else
	{
		this->m_record->Autologon = false;
		record->Autologon = false;
	}

	::GetPrivateProfileString(_T("Config"), _T("Try"), _T("3"), buff, len, path);		//��������,Ĭ��Ϊ 3
	sscanf_s(buff, "%d", &(this->m_record->Try));
	sscanf_s(buff, "%d", &(record->Try));

	::GetPrivateProfileString(_T("Config"), _T("netcard"), _T("0"), buff, len, path);		//��ǰ��ѡ���������,Ĭ��Ϊ0
	sscanf_s(buff, "%d", &(this->m_record->netcard));
	sscanf_s(buff, "%d", &(record->netcard));

	::GetPrivateProfileString(_T("Config"), _T("LastUser"), _T("0"), buff, len, path);		//��ǰ�û��˺����,Ĭ��Ϊ0
	sscanf_s(buff, "%d", &(this->m_record->LastUser));
	sscanf_s(buff, "%d", &(record->LastUser));

	::GetPrivateProfileString(_T("Config"), _T("username"), _T(""), buff, len, path);		//�����û��˺�,Ĭ��Ϊ""
	memcpy_s(this->m_record->username, len, buff, len);
	memcpy_s(record->username, len, buff, len);

	::GetPrivateProfileString(_T("Config"), _T("userPWD"), _T(""), buff, len, path);		//�����û�����,Ĭ��Ϊ""
	memcpy_s(this->m_record->userPWD, len, buff, len);
	memcpy_s(record->userPWD, len, buff, len);
	return TRUE;
}

int CIniFile::SetRrcord(char *path)			//д�������ļ�
{
	int len = 10;
	char buff[10] = { 0 };
	sprintf_s(buff, len, "%d", CUI::GetTimeout());
	WritePrivateProfileString(_T("Config"), _T("Timeout"), buff, path);			//��ʱ����

	if (CUI::GetRememberPWD())
		strcpy_s(buff, len, "true");
	else
		strcpy_s(buff, len, "false");
	WritePrivateProfileString(_T("Config"), _T("RememberPWD"), buff, path);		//�Ƿ��ס����

	if (CUI::GetAutorun())
		strcpy_s(buff, len, "true");
	else
		strcpy_s(buff, len, "false");
	WritePrivateProfileString(_T("Config"), _T("Autorun"), buff, path);			//�Ƿ񿪻�����

	if (CUI::GetAutologon())
		strcpy_s(buff, len, "true");
	else
		strcpy_s(buff, len, "false");
	WritePrivateProfileString(_T("Config"), _T("Autologon"), buff, path);		//�Ƿ��Զ���¼

	sprintf_s(buff, len, "%d", CUI::GetTry());
	WritePrivateProfileString(_T("Config"), _T("Try"), buff, path);				//��������

	sprintf_s(buff, len, "%d", CUI::Getnetcard());
	WritePrivateProfileString(_T("Config"), _T("netcard"), buff, path);			//��ѡ�������

	sprintf_s(buff, len, "%d", CUI::GetLastUser());
	WritePrivateProfileString(_T("Config"), _T("LastUser"), buff, path);			//��ǰʹ���û������

	WritePrivateProfileString(_T("Config"), _T("username"), CUI::Getusername(), path);		//�����û��˺�

	WritePrivateProfileString(_T("Config"), _T("userPWD"), CUI::GetuserPWD(), path);		//�����û�����

	return TRUE;
}
