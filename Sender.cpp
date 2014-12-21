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
	//本机发出的包
	RES_START,				//本机上线包
		RES_ID,				//本机ID身份包
		RES_MD5,				//本机MD5回应包
		RES_KEY1,				//本机续网KEY1包
		RES_KEY2,				//本机续网KEY2包
		RES_LOGOFF								//本机下线包*/
	if (!this->m_adapterHandle)
	{
		//1000表示超时时间为1s	,65536=65535+1表示<=65536的数据包都接受下来,而任何一个协议的数据包都小于等于65535的
		//PCAP_OPENFLAG_PROMISCUOUS==1表示进入混杂模式   即只要局域网内的数据包都会接收下来
		char m_errorBuffer[PCAP_ERRBUF_SIZE];		//错误信息缓冲区
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

	//发送需要的数据包
	u_char* sendPacket = NULL;
	int len = this->m_packet->GetPacket(&sendPacket);
	if (pcap_sendpacket(this->m_adapterHandle, sendPacket, len) != 0)
	{
		return FALSE;
	}
	return TRUE;

}