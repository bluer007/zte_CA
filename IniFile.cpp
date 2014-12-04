#pragma once
#include "Public.h"
#include "IniFile.h"




CIniFile::CIniFile()
{
	this->m_record = new INI_RECORD();		//初始化m_record
}

CIniFile::~CIniFile()
{
	delete this->m_record;
}

int CIniFile::GetRecord(char *path, INI_RECORD *record)
{
	//如果找到配置文件则读取里面的记录,找不到或读取不了相应项则用默认的配置信息
	int len = 100;
	char buff[100] = { 0 };
	::GetPrivateProfileString(_T("Config"), _T("Timeout"), _T("200"), buff, len, path);		//超时秒数,海大中兴2014.11日测试是180秒,现保险设为200秒
	sscanf_s(buff, "%d", &(this->m_record->Timeout));
	sscanf_s(buff, "%d", &(record->Timeout));

	::GetPrivateProfileString(_T("Config"), _T("RememberPWD"), _T("true"), buff, len, path);	//是否记住密码,默认 true
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

	::GetPrivateProfileString(_T("Config"), _T("Autorun"), _T("false"), buff, len, path);		//是否开机运行,默认为 false
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

	::GetPrivateProfileString(_T("Config"), _T("Autologon"), _T("false"), buff, len, path);		//是否自动登录,默认为false
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

	::GetPrivateProfileString(_T("Config"), _T("Try"), _T("3"), buff, len, path);		//重连次数,默认为 3
	sscanf_s(buff, "%d", &(this->m_record->Try));
	sscanf_s(buff, "%d", &(record->Try));

	::GetPrivateProfileString(_T("Config"), _T("netcard"), _T("0"), buff, len, path);		//当前所选网卡的序号,默认为0
	sscanf_s(buff, "%d", &(this->m_record->netcard));
	sscanf_s(buff, "%d", &(record->netcard));

	::GetPrivateProfileString(_T("Config"), _T("LastUser"), _T("0"), buff, len, path);		//当前用户账号序号,默认为0
	sscanf_s(buff, "%d", &(this->m_record->LastUser));
	sscanf_s(buff, "%d", &(record->LastUser));

	::GetPrivateProfileString(_T("Config"), _T("username"), _T(""), buff, len, path);		//所有用户账号,默认为""
	memcpy_s(this->m_record->username, len, buff, len);
	memcpy_s(record->username, len, buff, len);

	::GetPrivateProfileString(_T("Config"), _T("userPWD"), _T(""), buff, len, path);		//所有用户密码,默认为""
	memcpy_s(this->m_record->userPWD, len, buff, len);
	memcpy_s(record->userPWD, len, buff, len);
	return TRUE;
}

int CIniFile::SetRrcord(char *path)			//写入配置文件
{
	int len = 10;
	char buff[10] = { 0 };
	sprintf_s(buff, len, "%d", CUI::GetTimeout());
	WritePrivateProfileString(_T("Config"), _T("Timeout"), buff, path);			//超时秒数

	if (CUI::GetRememberPWD())
		strcpy_s(buff, len, "true");
	else
		strcpy_s(buff, len, "false");
	WritePrivateProfileString(_T("Config"), _T("RememberPWD"), buff, path);		//是否记住密码

	if (CUI::GetAutorun())
		strcpy_s(buff, len, "true");
	else
		strcpy_s(buff, len, "false");
	WritePrivateProfileString(_T("Config"), _T("Autorun"), buff, path);			//是否开机运行

	if (CUI::GetAutologon())
		strcpy_s(buff, len, "true");
	else
		strcpy_s(buff, len, "false");
	WritePrivateProfileString(_T("Config"), _T("Autologon"), buff, path);		//是否自动登录

	sprintf_s(buff, len, "%d", CUI::GetTry());
	WritePrivateProfileString(_T("Config"), _T("Try"), buff, path);				//重连次数

	sprintf_s(buff, len, "%d", CUI::Getnetcard());
	WritePrivateProfileString(_T("Config"), _T("netcard"), buff, path);			//所选网卡序号

	sprintf_s(buff, len, "%d", CUI::GetLastUser());
	WritePrivateProfileString(_T("Config"), _T("LastUser"), buff, path);			//当前使用用户名序号

	WritePrivateProfileString(_T("Config"), _T("username"), CUI::Getusername(), path);		//所有用户账号

	WritePrivateProfileString(_T("Config"), _T("userPWD"), CUI::GetuserPWD(), path);		//所有用户密码

	return TRUE;
}
