#include "Public.h"
#include "Reve.h"
#include "UI.h"


CRever::CRever(char* adapter, int time, int timeout)
{
	this->adapter_description = new char[strlen(adapter)+1];
	strcpy_s(this->adapter_description, strlen(adapter) + 1, adapter);
	this->m_isStop = true;				//���ý�������ʼʱ��ֹͣ��
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
	//���ݴ���������������,���ظ�����������name,
	//�ɹ�����0	,ʧ�ܷ��ر�ʾnameӦ���еĴ�С,	-1��ʾ��������

	pcap_if_t *alldevs;		//�����������б�
	pcap_if_t *d;			//��ʱ����������
	int i = 0, result=-1;	//result��Ϊ��������ֵ,Ĭ��-1,�����
	char errbuf[PCAP_ERRBUF_SIZE];		//������Ϣ������

	/* ȡ�������б� */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		//����Ѱ������ʧ�ܣ��밲װ�������°�װWinpap	
		result=-1;			//��������,����-1
		CUI::OutError(NO_WINPAP);		//���������Ϣ
	}

	/* ����б� */
	for (d = alldevs; d; d = d->next)
	{
		i++;
		if (strcmp(d->description, description) == 0)
		{
			if (namelen >= strlen(d->name))
			{	//�����������name��С�㹻
				//���ݴ�����������������  �ҵ������ø�����������
				strcpy_s(name, namelen, d->name);
				result= 0;		//�ɹ�����0
			}
			else
			{
				result=strlen(d->name);		//��������name�������򷵻�nameӦ�д�С
			}
		}
			
	}
	if (i == 0)
	{
		////���󣺿���û������������������װWinpap
		result= -1;			//�������󷵻�-1
		CUI::OutError(NO_Adapter);
	}

	/* ���ǲ�����Ҫ�б���, �ͷ� */
	pcap_freealldevs(alldevs);
	return result;
}


int CRever::StartReve(char* captured)
{
	this->m_isStop = false;
	int usetime = 0;
	char* temp_adapter_name=(char*)calloc(1,sizeof(char));			//calloc���ʼ��ΪĬ��ֵ,malloc�����ʼ��
	int namelen = this->FindAdapter(this->adapter_description, temp_adapter_name, 0);	//���ڴ���0,���Կ϶�FindAdapter����name��Ҫ�Ĵ�Сlen

	char* adapter_name = (char*)realloc(temp_adapter_name,namelen);		//������������adapter_name
	int result = this->FindAdapter(this->adapter_description, adapter_name, namelen);
	if (0 == result)
	{
		//FindAdapter==0��ʾ���гɹ�
		while (this->m_isStop || usetime <= this->m_timeout)
		{
			pcap_open_live(adapter_name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, m_errorBuffer);
		}
	}
	else 
		return FALSE;		

	

	
	return TRUE;
}