#pragma once
#define WIN32
#include "pcap.h" 
#include <Windows.h>
#include <tchar.h>

#include "pcap.h"
#include <Iphlpapi.h>		//为了IP_ADAPTER_INFO结构体可以找到定义,用户获取网卡mac地址
#include "resource.h"
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib,"wpcap.lib")


enum OK_ERROR
{
	//error
	NO_WINPAP				=1	,		//错误：寻找网卡失败,Winpap出错！请安装或者重新安装Winpap		
	NO_Description				,		//警告：网卡没有一个有效的描述
	NO_Adapter					,		//错误：可能没有网卡，可以试试重装Winpap
	NO_IP						,		//错误:获取IP超时,请重新试试

	NO_Selecte_Adapter			,		//提醒:请选择有效网卡 ^_^ 
	NO_Username					,		//提醒:请输入有效账号名 ^_^
	NO_Pwd						,		//提醒:请输入有效密码
	NO_Online					,		//状态:掉线

	//success
	OK_Init					=101,		//初始化成功，成功加载网卡！
	OK_IP						,		//成功获取IP
	OK_Success					,		//连网成功, 可以上网罗~~~
	OK_Online							//状态:连网

};


//数据包类型常量
enum Packet_Type
{
	//服务器发来的包
	UN_KNOWN			=1	,				//服务器发来的不知名包
	REQ_ID					,				//服务器请求ID身份包
	REQ_MD5					,				//服务器请求MD5挑战包
	REQ_SUCCESS				,				//服务器响应SUCCESS包
	REQ_KEY					,				//服务器请求EAPOL_KEY包
	REQ_FAILURE				,				//服务器响应failure包,用于下线logoff包的确认,或者,密码错误等原因			

	//本机发出的包
	RES_START				,				//本机上线包
	RES_ID					,				//本机ID身份包
	RES_MD5					,				//本机MD5回应包
	RES_KEY1				,				//本机续网KEY1包
	RES_KEY2				,				//本机续网KEY2包
	RES_LOGOFF								//本机下线包
};



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

