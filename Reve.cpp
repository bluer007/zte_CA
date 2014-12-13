#include "Public.h"
#include "Reve.h"
#include "UI.h"


CRever::CRever(const char* AdapterName)
{
	this->adapter_name = new char[strlen(AdapterName) + 1];
	strcpy_s(this->adapter_name, strlen(AdapterName) + 1, AdapterName);

}

CRever::~CRever()
{
	delete this->adapter_name;
}

/*
int CRever::FindAdapter(const char* description, char* name,int namelen)
{
	//int CRever::FindAdapter(const char* description, char* name,int namelen)
	//根据传进来的网卡描述,返回该网卡的名字name,
	//成功返回0	,失败返回表示name应该有的大小,	-1表示其他错误

	pcap_if_t *alldevs;		//网卡适配器列表
	pcap_if_t *d;			//临时网卡适配器
	int i = 0, result=-1;	//result作为函数返回值,默认-1,表错误
	char errbuf[PCAP_ERRBUF_SIZE];		//错误信息缓冲区

	/ * 取得网卡列表 * /
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//错误：寻找网卡失败！请安装或者重新安装Winpap	
		result=-1;			//其他错误,返回-1
		CUI::OutError(NO_WINPAP);		//输出错误信息
	}

	/ * 输出列表 * /
	for (d = alldevs; d; d = d->next)
	{
		i++;
		if (strcmp(d->description, description) == 0)
		{
			if (namelen >= ((int)strlen(d->name)+1))
			{	//如果传进来的name大小足够
				//根据传进来的网卡的描述  找到并设置该网卡的名字
				strcpy_s(name, namelen, d->name);
				result= 0;		//成功返回0
			}
			else
			{
				result=(int)strlen(d->name)+1;		//传进来的name不够大则返回name应有大小,包括最后的/0字节
			}
		}
			
	}
	if (i == 0)
	{
		////错误：可能没有网卡，可以试试重装Winpap
		result= -1;			//其他错误返回-1
		CUI::OutError(NO_Adapter);
	}

	/ * 我们不再需要列表了, 释放 * /
	pcap_freealldevs(alldevs);
	return result;
}*/

int CRever::GetMAC(char* mac)
{
	//寻找所选的网卡的MAC
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	do 
	{
		//pcap_findalldevs
		//所得的adapter_name格式是		\Device\NPF_{ 9E4EB639 - 5BA7 - 450B - AB23 - B5D959BCFD79 }
		//adapter->description格式是		Realtek PCIe GBE Family Controller(只有英文哦)

		//GetAdaptersInfo
		//所得的adapter_name格式是{ 9E4EB639 - 5BA7 - 450B - AB23 - B5D959BCFD79 }
		//adapter->description格式是		Realtek PCIe GBE 系列控制器(有中文哦)
		if (strstr(this->adapter_name, pAdapterInfo->AdapterName) > 0)
			break;
	} while (pAdapterInfo = pAdapterInfo->Next);
	if (ERROR_SUCCESS == dwStatus)
	{
		_strset_s(mac, strlen(mac) + 1, '\0');
		char temp[5] = { 0 };
		for (int i = 0; i < (int)pAdapterInfo->AddressLength; i++)
		{
			if (i < (int)pAdapterInfo->AddressLength - 1)
			{
				sprintf_s(temp, "%02x:", pAdapterInfo->Address[i]);
				strcat_s(mac,strlen(mac) +strlen(temp)+1 , temp);
			}
			else
			{
				sprintf_s(temp, "%02x\0", pAdapterInfo->Address[i]);
				strcat_s(mac, strlen(mac) + strlen(temp) + 1, temp);
			}
		}
		return TRUE;
	}
	else
		return FALSE;
	
}

int CRever::StartReve(const u_char** captured, int* len)
{
		*len = 0;
		char m_errorBuffer[PCAP_ERRBUF_SIZE];		//错误信息缓冲区
		//1000表示超时时间为1s	,65536=65535+1表示<=65536的数据包都接受下来,而任何一个协议的数据包都小于等于65535的
		//PCAP_OPENFLAG_PROMISCUOUS==1表示进入混杂模式   即只要局域网内的数据包都会接收下来
		pcap_t * adapterHandle = pcap_open_live(this->adapter_name, 65536, 1, 9000, m_errorBuffer);
		if (!adapterHandle)
			return FALSE;
		
		char mac[50] = {0};
		char FilterStr[80] = { 0 };			//过滤包字符串
		struct bpf_program	mfcode;			//存放编译以后的规则

		if (!this->GetMAC(mac))	
			return FALSE;

		sprintf_s(FilterStr, "(ether proto 0x0800) and (ether dst host %s)", mac);		
		//只接受802.1x协议(0x888e)和目的地址是自己mac的数据包		0x0800为ip协议
		if (pcap_compile(adapterHandle, &mfcode, FilterStr, 1, 0xffffff) == -1		//0xffffff表示子网掩码
			|| pcap_setfilter(adapterHandle, &mfcode) == -1)
			return FALSE;

		pcap_pkthdr* header;		//报文头(winpcap自己加上去的)
		int res = 0;

		if ((res= pcap_next_ex(adapterHandle, &header, captured)) >= 0)
		{
			*len = header->len;

			if (res = 0)
			{
				printf("sorry");
				
			}

			printf("\n\n%d\n",*captured);
			printf("\n\n开始--\n");
			int num = 0;
			while (num < header->len)
			{
				printf("%d-", num);
				printf("% 02x  ", (*captured)[num++]);
			}

			return TRUE;
		}
		else
			return FALSE;
}