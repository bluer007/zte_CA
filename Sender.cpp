#include "Public.h"
#include "Sender.h"

CSender::CSender(const char* AdapterName)
{
	this->adapter_name = new char[strlen(AdapterName) + 1]{0};
	strcpy_s(this->adapter_name, strlen(AdapterName) + 1, AdapterName);

	this->m_adapterHandle = NULL;
	this->m_packet = NULL;
	this->m_maker = NULL;
}

CSender::CSender(const pcap_t* adapterHandle)
{
	this->m_maker = NULL;
	this->m_packet = NULL;
	this->adapter_name = NULL;
	if (adapterHandle)
		this->m_adapterHandle = (pcap_t*)adapterHandle;
}

CSender::~CSender()
{
	if (this->adapter_name)
		delete this->adapter_name;
	if (this->m_maker)
		delete this->m_maker;

}

int CSender::StartSend(Packet_Type type, const u_char* captured)
{
	/*
	//���������İ�
	RES_START,				//�������߰�
		RES_ID,				//����ID��ݰ�
		RES_MD5,				//����MD5��Ӧ��
		RES_KEY1,				//��������KEY1��
		RES_KEY2,				//��������KEY2��
		RES_LOGOFF								//�������߰�*/
	if (!this->m_adapterHandle)
	{
		//1000��ʾ��ʱʱ��Ϊ1s	,65536=65535+1��ʾ<=65536�����ݰ�����������,���κ�һ��Э������ݰ���С�ڵ���65535��
		//PCAP_OPENFLAG_PROMISCUOUS==1��ʾ�������ģʽ   ��ֻҪ�������ڵ����ݰ������������
		char m_errorBuffer[PCAP_ERRBUF_SIZE];		//������Ϣ������
		this->m_adapterHandle = pcap_open_live(this->adapter_name, 65536, 1, 1000, m_errorBuffer);
	}
	if (!this->m_maker)
		this->m_maker = new CMaker();

	switch (type)
	{
	case RES_START:
	case RES_ID:
	case RES_MD5:
	case RES_KEY1:
	case RES_LOGOFF:
	{
		int isOK = FALSE;
		this->m_packet = this->m_maker->MakePacket(type, captured, &isOK);
		if (!isOK)
		{
			this->m_packet = NULL;
			return FALSE;
		}
		break;
	}
	default:
		return FALSE;
	}

	//������Ҫ�����ݰ�
	u_char* sendPacket = NULL;
	int len = this->m_packet->GetPacket(&sendPacket);
	if (pcap_sendpacket(this->m_adapterHandle, sendPacket, len) != 0)
	{
		return FALSE;
	}
	return TRUE;

}