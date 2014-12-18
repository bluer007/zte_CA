#pragma once
#include "Public.h"
#include "Maker.h"

class CSender
{
public:
	CSender(const char* AdapterName);
	CSender(const pcap_t* adapterHandle);

	~CSender();

	int StartSend(Packet_Type type, const u_char* captured);

private:
	CMaker* m_maker;
	CPacket* m_packet;
	char* adapter_name;
	pcap_t* m_adapterHandle;
};