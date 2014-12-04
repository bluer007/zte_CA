#include "Public.h"
#include "Reve.h"
#include "UI.h"


CRever::CRever(char* adapter, int time, int timeout)
{
	this->adapter_description = new char[strlen(adapter)+1];
	strcpy_s(this->adapter_description, strlen(adapter) + 1, adapter);
	this->m_isStop = true;				//设置接收器开始时是停止的
	this->m_time = time;
	this->m_timeout = timeout;
}

CRever::~CRever()
{
	delete this->adapter_description;
}

int CRever::FindAdapter(const char* description, char* name,int namelen)
{
	//int CRever::FindAdapter(const char* description, char* name,int namelen)
	//根据传进来的网卡描述,返回该网卡的名字name,
	//成功返回0	,失败返回表示name应该有的大小,	-1表示其他错误

	pcap_if_t *alldevs;		//网卡适配器列表
	pcap_if_t *d;			//临时网卡适配器
	int i = 0, result=-1;	//result作为函数返回值,默认-1,表错误
	char errbuf[PCAP_ERRBUF_SIZE];		//错误信息缓冲区

	/* 取得网卡列表 */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//错误：寻找网卡失败！请安装或者重新安装Winpap	
		result=-1;			//其他错误,返回-1
		CUI::OutError(NO_WINPAP);		//输出错误信息
	}

	/* 输出列表 */
	for (d = alldevs; d; d = d->next)
	{
		i++;
		if (strcmp(d->description, description) == 0)
		{
			if (namelen >= strlen(d->name))
			{	//如果传进来的name大小足够
				//根据传进来的网卡的描述  找到并设置该网卡的名字
				strcpy_s(name, namelen, d->name);
				result= 0;		//成功返回0
			}
			else
			{
				result=strlen(d->name);		//传进来的name不够大则返回name应有大小
			}
		}
			
	}
	if (i == 0)
	{
		////错误：可能没有网卡，可以试试重装Winpap
		result= -1;			//其他错误返回-1
		CUI::OutError(NO_Adapter);
	}

	/* 我们不再需要列表了, 释放 */
	pcap_freealldevs(alldevs);
	return result;
}


int CRever::StartReve(char* captured)
{
	this->m_isStop = false;
	int usetime = 0;
	char* temp_adapter_name=(char*)calloc(1,sizeof(char));			//calloc会初始化为默认值,malloc不会初始化
	int namelen = this->FindAdapter(this->adapter_description, temp_adapter_name, 0);	//由于传入0,所以肯定FindAdapter返回name需要的大小len

	char* adapter_name = (char*)realloc(temp_adapter_name,namelen);		//真正用来保存adapter_name
	int result = this->FindAdapter(this->adapter_description, adapter_name, namelen);
	if (0 == result)
	{
		//FindAdapter==0表示运行成功
		while (this->m_isStop || usetime <= this->m_timeout)
		{
			pcap_open_live(adapter_name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, m_errorBuffer);
		}
	}
	else 
		return FALSE;		

	

	
	return TRUE;
}