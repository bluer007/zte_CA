#include "Maker.h"
#include "Reve.h"
#include "UI.h"
#include "md5.h"
#include "rc4.h"

////////////////////////////�򵥹���ģʽ--��Ʒ����ģ��
CPacket::CPacket()
{
	this->m_packet = NULL;
	this->m_packetLen = 0;
}

CPacket::~CPacket()
{
	if (this->m_packet)
	{
		delete this->m_packet;
	}
}

int CPacket::GetPacket(u_char** packet)
{
	if (this->m_packet)
	{
		*packet = this->m_packet;
		return this->m_packetLen;
	}
	return FALSE;
}


/////////////////////////////////////�򵥹���ģʽ--����ģ��
CMaker::CMaker()
{
	this->m_type = UN_KNOWN;
}

CMaker::~CMaker()
{

}

CPacket* CMaker::MakePacket(const Packet_Type type, const u_char* src_packet, int* IsOK)
{
/*
	//���������İ�
	RES_START,				//�������߰�
		RES_ID,				//����ID��ݰ�
		RES_MD5,				//����MD5��Ӧ��
		RES_KEY1,				//��������KEY1��
		RES_KEY2,				//��������KEY2��
		RES_LOGOFF				//�������߰�
*/
	this->m_type = type;
	*IsOK = TRUE;
	switch (type)
	{
	case RES_START:
		return new CMake_Start(src_packet);
		break;
	case RES_ID:
		return new CMake_ID(src_packet);
		break;
	case RES_MD5:
		return new CMake_MD5(src_packet);
		break;
	case RES_KEY1:
		return new CMake_KEY1(src_packet);
		break;
	case RES_LOGOFF:
		return new CMake_LOGOFF(src_packet);
		break;
	}
	*IsOK = FALSE;
	this->m_type = UN_KNOWN;
	return NULL;
}


//�򵥹���ģʽ--��Ʒʵ��ģ��
//////////////////////////////////////////////////////////////////////////
CMake_Start::CMake_Start(const u_char* src_packet)
{
	//const u_char BroadcastAddr[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // �㲥MAC��ַ
	//const u_char MultcastAddr[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x03 }; // �ಥMAC��ַ
	this->m_packet = new u_char[18]
	{
		0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,			//�ಥMAC��ַ(802.1x�涨)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//����mac��ַ
		0x88, 0x8e,									//802.1xЭ��
		0x01,										//802.1x�汾
		0x01,										//��ʾstart
		0x00, 0x00									//����(0)
	};
	this->m_packetLen = 18;

	u_char my_mac[8] = {0};
	assert(CRever::GetMAC(my_mac));
	//��ֵ����mac
	memcpy_s(this->m_packet + 6 , 18, my_mac, 6);

}

CMake_Start::~CMake_Start()
{
	
}

int CMake_Start::GetPacket(u_char** packet)
{
	if (this->m_packet)
	{
		*packet = this->m_packet;
		return this->m_packetLen;
	}
	return FALSE;
}



//////////////////////////////////////////////////////////////////////////
//����ID��ݰ�
CMake_ID::CMake_ID(const u_char* src_packet)
{
	if (src_packet)
	{
		const char* name = CUI::Getusername();
		const int namelen = strlen(name);
		const int len = 23 + namelen;		//23��������֪������ֽ���
		this->m_packet = new u_char[len]
		{
			0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,			//�Է�MAC
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//�Լ�MAC
			0x88, 0x8e,									//����Э������(802.1x)
			0x01,										//Version: 1
			0x00,										//Type: EAP Packet (0)
			0x00, 0x00,									//���ȣ�ʮ�����Ʊ�ʾ��һ�ʼ�����ݰ�������ռ����
			0x02,										//Code: Response (2)
			0x00,										//Id: �ɷ����İ�����
			0x00, 0x00,									//Length:  (ͬ����ĳ�����)
			0x01,										//Type: Identity [RFC3748] (1)
			//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //����Լ����˺ţ���ascii�룬��У�˺ų���Ϊ11
			//(�ָ���Getusername()���ص��˺ŵ��˺ų���)
		};
		this->m_packetLen = len;

		//���Ʊ���mac�������ķ��Ͱ�
		memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);
		
		char str[1] = { namelen + 5 };
		
		//��ֵ ���� ��
		memcpy_s(this->m_packet + 17, len, str, 1);
		memcpy_s(this->m_packet + 21, len, str, 1);

		//��ֵ id ��
		memcpy_s(this->m_packet + 19, len, &src_packet[19], 1);

		//��ֵ �˺� ��
		memcpy_s(this->m_packet + 23, len, name, namelen);


	}
}

CMake_ID::~CMake_ID()
{

}

int CMake_ID::GetPacket(u_char** packet)
{
	if (this->m_packet)
	{
		*packet = this->m_packet;
		return this->m_packetLen;
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//����MD5��ս��(�����ͼ�������)
CMake_MD5::CMake_MD5(const u_char* src_packet)
{
	const char* name = CUI::Getusername();
	const int namelen = strlen(name);
	const int len = 40 + namelen;		//40��������֪������ֽ���
	this->m_packet = new u_char[len]
	{
		0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,			//�Է�MAC
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//�Լ�MAC
		0x88, 0x8e,								//����
		0x01,									//Version: 1
		0x00,									//Type: EAP Packet (0)
		0x00, 0x00,								//���ȣ��������һ�ʼ�������ռ�ֽ���
		0x02,									//Code: Response (2)
		0x00,									//Id: ��������md5��ս�����һ��
		0x00, 0x00,								//Length: ͬ����ĳ���һ��
		0x04,									//Type: MD5-Challenge [RFC3748] (4)
		0x10,									//Value-Size: 16
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//16�ֽڵ�MD5��ǰ8�ֽ�
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//16�ֽڵ�MD5�ĺ�8�ֽ�
		//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //����Լ����˺ţ���ascii�룬��У�˺ų���Ϊ11
	};
	this->m_packetLen = len;

	//���Ʊ���mac�������ķ��Ͱ�
	memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);

	char str[1] = { namelen + 22 };
	
	//��ֵ ���� ��
	memcpy_s(this->m_packet + 17, len, str, 1);
	memcpy_s(this->m_packet + 21, len, str, 1);

	//��ֵ id ��
	memcpy_s(this->m_packet + 19, len, &src_packet[19], 1);

	//��ֵ �˺� ��
	memcpy_s(this->m_packet + 40, len, name, namelen);

	//����MD5
	u_char MD5Res[16];
	MD5_CTX context;
	BYTE	msgbuf[128] = { 0 };
	int i = 0, j = 0;
	char* pwd = CUI::GetuserPWD();
	
	//������MD5-Challenge�İ�ID
	msgbuf[i++] = src_packet[19];

	//�����ǵ�½����
	while (j< (int)strlen(pwd))
	{
		msgbuf[i++] = pwd[j++];
	}

	//������������MD5-Challenge
	memcpy_s(msgbuf + i, 128, &src_packet[24], 16);
	i += 16;

	//ȡ��MD5��ϢժҪ�������MD5Res��
	MD5Init(&context);
	MD5Update(&context, msgbuf, i);
	MD5Final(MD5Res, &context);
	//MD5-Challenge:
	//��ֵMD5��(����md5��Ϊ(id+����+md5����challenge)���������md5)
	memcpy_s(this->m_packet + 24, len, MD5Res, 16);

}

CMake_MD5::~CMake_MD5()
{

}

int CMake_MD5::GetPacket(u_char** packet)
{
	if (this->m_packet)
	{
		*packet = this->m_packet;
		return this->m_packetLen;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//��������key1��
CMake_KEY1::CMake_KEY1(const u_char* src_packet)
{
	const int len = 78;
	this->m_packet = new u_char[len]
	{
			0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,						//�Է�MAC
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,						//�Լ�MAC
			0x88, 0x8e,												//Э������
			0x01,													//Version: 1
			0x03,													//Type: EAP Packet (3)
			0x00, 0x3c,												//���ȣ�3c����10���Ƶ�60
			0x01,													//Descriptor Type: RC4 Descriptor (1)
			0x00, 0x10,												//key length,��ʾʮ���Ƶ�16
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//Replay Counter: 8�ֽ�
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16�ֽڵ�Key IV ǰ8�ֽ�
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16�ֽڵ�Key IV ��8�ֽ�
			0x00,													//index	
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16�ֽڵ�Key Signature ǰ8�ֽ�
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16�ֽڵ�Key Signature ��8�ֽ�
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16�ֽڵ�Key ǰ8�ֽ�
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00			//16�ֽڵ�Key ��8�ֽ�
	};
	this->m_packetLen = len;

	//���Ʊ���mac�������ķ��Ͱ�
	memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);

	//��ֵ Replay Counter ��  Key IV ��,ԭ�����ƣ���24�ֽ�
	memcpy_s(this->m_packet + 21, len, src_packet + 21, 24);

	//��ֵindex��
	memcpy_s(this->m_packet + 45, len, src_packet + 45, 1);

	
	u_char enckey[] = 
	{	//�������˵ļ�������(��ͬѧУ���ܲ�ͬ)
		0x02, 0x0E, 0x05, 0x04, 0x66, 0x40, 0x19, 0x75
		, 0x06, 0x06, 0x00, 0x16, 0xD3, 0xF3, 0xAC, 0x02		//luzj��������
		/*0x02, 0x0e, 0x05, 0x04, 0x18, 0x0f, 0x02, 0x0a,
		0x06, 0x06, 0x04, 0x7d, 0x7b, 0x98, 0x17, 0xc1*/
	};
	u_char wholekey[20];
	memcpy_s(wholekey, 20, src_packet + 29, 16);		//src_packet + 29��Key IV��ȫ��16���ֽ�
	memcpy_s(wholekey + 16, 20, src_packet + 41, 4);		//src_packet + 41��Key IV����4���ֽ�
	int keylen = 16;
	struct rc4_state s;
	rc4_setup(&s, wholekey, 20);
	rc4_crypt(&s, enckey, keylen);
	//��ֵkey��(��Key IV+Key IV���4���ֽ�,����rc4�㷨����)
	memcpy_s(&this->m_packet[62], len, enckey, 16);

	//ʹ��hmac_md5�㷨����Key Signature�������ڰ���У��
	u_char deckey[64] = { 0 };
	u_char encDat[64] = {0};
	//enckey[0] = src_packet[45];	//++++
	memcpy_s(encDat, 64, this->m_packet + 14, 64);		//src_packet + 14���Version�ʼ��������64�ֽ�

	printf("\n\n encDat____\n");
	for (int i = 1; i <=64;i++)
	{
		if ((i % 16) == 1)
			printf("\n");
		printf("%02x ", encDat[i-1]);
	}	printf("\n\n deckey-----\n");


	hmac_md5(encDat, 64, (unsigned char*)&src_packet[45], 1, deckey);		//src_packet[45]��index��

	for (int i = 1; i <= 64; i++)
	{
		if ((i % 16) == 1)
			printf("\n");
		printf("%02x ", deckey[i-1]);
	}printf("\n\n");


	//��ֵKey Signature�� (��version��ʼ������Ϊֹ�İ�����(Key Signature���0)����hmac��md5����(��Կ��index��)�Ľ��.)
	memcpy_s(this->m_packet + 46, len, deckey, 16);
}

CMake_KEY1::~CMake_KEY1()
{

}

int CMake_KEY1::GetPacket(u_char** packet)
{
	if (this->m_packet)
	{
		*packet = this->m_packet;
		return this->m_packetLen;
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//����logoff���߰�
CMake_LOGOFF::CMake_LOGOFF(const u_char* src_packet)
{
	//const u_char BroadcastAddr[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // �㲥MAC��ַ
	//const u_char MultcastAddr[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x03 }; // �ಥMAC��ַ
	this->m_packet = new u_char[18]
	{
		0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,			//�ಥMAC��ַ(802.1x�涨)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//����mac��ַ
		0x88, 0x8e,									//802.1xЭ��
		0x01,										//802.1x�汾
		0x02,										//��ʾlogoff
		0x00, 0x00									//����(0)
	};
	this->m_packetLen = 18;

	u_char my_mac[8] = { 0 };
	assert(CRever::GetMAC(my_mac));
	//��ֵ����mac
	memcpy_s(this->m_packet + 6, 18, my_mac, 6);
}

CMake_LOGOFF::~CMake_LOGOFF()
{

}

int CMake_LOGOFF::GetPacket(u_char** packet)
{
	if (this->m_packet)
	{
		*packet = this->m_packet;
		return this->m_packetLen;
	}
	return FALSE;
}