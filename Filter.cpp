#include "Public.h"
#include "Filter.h"


CFilter::CFilter()
{

};

CFilter::~CFilter()
{

};

Packet_Type CFilter::StartFilter(const u_char* captured)
{
	enum EAP_Code
	{					//根据802.1x协议所得
		EAP_PACKET		= 0x00,
		KEY				= 0x03,
		REQUEST			= 0x01,
		SUCCESS			= 0x03,
		FAILURE			= 0x04,
		IDENTITY		= 0x01,
		MD5				= 0x04,
		RC4				= 0x01
	};
	switch (captured[15])
	{
	case EAP_PACKET:
		switch (captured[18])
		{
		case REQUEST:
			switch (captured[22])
			{
			case IDENTITY:
				return REQ_ID;
				break;
			case MD5:
				return REQ_MD5;
				break;
			}
			break;
		case SUCCESS:
			return REQ_SUCCESS;
			break;
		case FAILURE:
			return REQ_FAILURE;
			break;
		}
		break;
	case KEY:
		switch (captured[18])
		{
		case RC4:
			return REQ_KEY;
			break;
		}
		break;
	}

	return UN_KNOWN;
};