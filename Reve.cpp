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
	//���ݴ���������������,���ظ�����������name,
	//�ɹ�����0	,ʧ�ܷ��ر�ʾnameӦ���еĴ�С,	-1��ʾ��������

	pcap_if_t *alldevs;		//�����������б�
	pcap_if_t *d;			//��ʱ����������
	int i = 0, result=-1;	//result��Ϊ��������ֵ,Ĭ��-1,�����
	char errbuf[PCAP_ERRBUF_SIZE];		//������Ϣ������

	/ * ȡ�������б� * /
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//����Ѱ������ʧ�ܣ��밲װ�������°�װWinpap	
		result=-1;			//��������,����-1
		CUI::OutError(NO_WINPAP);		//���������Ϣ
	}

	/ * ����б� * /
	for (d = alldevs; d; d = d->next)
	{
		i++;
		if (strcmp(d->description, description) == 0)
		{
			if (namelen >= ((int)strlen(d->name)+1))
			{	//�����������name��С�㹻
				//���ݴ�����������������  �ҵ������ø�����������
				strcpy_s(name, namelen, d->name);
				result= 0;		//�ɹ�����0
			}
			else
			{
				result=(int)strlen(d->name)+1;		//��������name�������򷵻�nameӦ�д�С,��������/0�ֽ�
			}
		}
			
	}
	if (i == 0)
	{
		////���󣺿���û������������������װWinpap
		result= -1;			//�������󷵻�-1
		CUI::OutError(NO_Adapter);
	}

	/ * ���ǲ�����Ҫ�б���, �ͷ� * /
	pcap_freealldevs(alldevs);
	return result;
}*/

int CRever::GetMAC(char* mac)
{
	//Ѱ����ѡ��������MAC
	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
	do 
	{
		//pcap_findalldevs
		//���õ�adapter_name��ʽ��		\Device\NPF_{ 9E4EB639 - 5BA7 - 450B - AB23 - B5D959BCFD79 }
		//adapter->description��ʽ��		Realtek PCIe GBE Family Controller(ֻ��Ӣ��Ŷ)

		//GetAdaptersInfo
		//���õ�adapter_name��ʽ��{ 9E4EB639 - 5BA7 - 450B - AB23 - B5D959BCFD79 }
		//adapter->description��ʽ��		Realtek PCIe GBE ϵ�п�����(������Ŷ)
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
		char m_errorBuffer[PCAP_ERRBUF_SIZE];		//������Ϣ������
		//1000��ʾ��ʱʱ��Ϊ1s	,65536=65535+1��ʾ<=65536�����ݰ�����������,���κ�һ��Э������ݰ���С�ڵ���65535��
		//PCAP_OPENFLAG_PROMISCUOUS==1��ʾ�������ģʽ   ��ֻҪ�������ڵ����ݰ������������
		pcap_t * adapterHandle = pcap_open_live(this->adapter_name, 65536, 1, 9000, m_errorBuffer);
		if (!adapterHandle)
			return FALSE;
		
		char mac[50] = {0};
		char FilterStr[80] = { 0 };			//���˰��ַ���
		struct bpf_program	mfcode;			//��ű����Ժ�Ĺ���

		if (!this->GetMAC(mac))	
			return FALSE;

		sprintf_s(FilterStr, "(ether proto 0x0800) and (ether dst host %s)", mac);		
		//ֻ����802.1xЭ��(0x888e)��Ŀ�ĵ�ַ���Լ�mac�����ݰ�		0x0800ΪipЭ��
		if (pcap_compile(adapterHandle, &mfcode, FilterStr, 1, 0xffffff) == -1		//0xffffff��ʾ��������
			|| pcap_setfilter(adapterHandle, &mfcode) == -1)
			return FALSE;

		pcap_pkthdr* header;		//����ͷ(winpcap�Լ�����ȥ��)
		int res = 0;

		if ((res= pcap_next_ex(adapterHandle, &header, captured)) >= 0)
		{
			*len = header->len;

			if (res = 0)
			{
				printf("sorry");
				
			}

			printf("\n\n%d\n",*captured);
			printf("\n\n��ʼ--\n");
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