#pragma once
#include <UIlib.h>
/*
#include <tchar.h>  
#include <atlbase.h>*/
using namespace DuiLib;
#pragma comment(lib, "DuiLib.lib")


class CUI : public WindowImplBase
{
public:
	CUI();
	~CUI();
	int InitUI();

	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("XMLWnd");
	}

	virtual UILIB_RESOURCETYPE GetResourceType() const;//
	virtual LPCTSTR GetResourceID() const;	//
	virtual CDuiString GetSkinFile();	//
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual CDuiString GetSkinFolder();

	/////////////用户配置文件的读取和生成/////////////////////////////////////////////////////////////
	static int GetTimeout(){ return 200; };
	static bool GetRememberPWD(){ return true; };
	static bool GetAutorun(){ return false; };
	static bool GetAutologon(){ return false; };
	static int GetTry(){ return 3; };
	static int Getnetcard(){ return 1; };
	static int GetLastUser(){ return 0; };
	static char* Getusername(){ return "zjDSLhdjg150428"; };
	static char* GetuserPWD(){ return "20072008"; };
	//////////////////////////////////////////////////////////////////////////

	int AddAdapter(char* description){ return 0; };		//增加网卡的列表
	int SetAdapter(char* description){ return 0; };				//设置选中网卡
	static int OutError(int code);		//输出错误信息
	int OutOK(int code){ return 0; };			//输出正确或提醒信息
	
	int SetTimeout(const int* time){ return TRUE; };
	int SetRememberPWD(const bool* isRemember){ return true; };
	int SetAutorun(const bool* isAutoRun){ return true; };
	int SetAutologon(const bool* isAutologon){ return true; };
	int SetTry(const int* tryNum){ return true; };
	int Setnetcard(const int* num){ return true; };
	int SetLastUser(const int* num){ return true; };
	int Setusername(const char* name){ printf("当前账号是%s\n",name); return true; };
	int Addusername(const char* name){ printf("账号是%s\n",name); return true; };
	int SetuserPWD(const char* pwd){ printf("密码是%s\n", pwd);return true; };
};