#pragma once
#define WIN32
#include "pcap.h" 
#include <Windows.h>
#include <tchar.h>


#include "resource.h"
#pragma comment(lib,"wpcap.lib")

//error
#define		NO_WINPAP				1		//错误：寻找网卡失败,Winpap出错！请安装或者重新安装Winpap		
#define		NO_Description			2		//警告：网卡没有一个有效的描述
#define		NO_Adapter				3		//错误：可能没有网卡，可以试试重装Winpap



//success
#define		OK_Init					101		//初始化成功，成功加载网卡！




struct INI_RECORD			//ini文件的记录结构
{
	int Timeout;			//超时秒数
	bool RememberPWD;		//是否记住密码
	bool Autorun;			//是否开机运行
	bool Autologon;			//是否自动登录
	int Try;				//尝试连接次数
	int netcard;			//当前选中的网卡
	int LastUser;			//当前用户账户
	char username[100];			//所有用户账号
	char userPWD[100];			//所有用户密码
};

