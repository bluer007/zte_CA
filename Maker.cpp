#include "Maker.h"
#include "UI.h"
#include "md5.h"

////////////////////////////�򵥹���ģʽ--��Ʒ����ģ��
CPacket::CPacket()
{
	this->m_packet = NULL;
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
		return TRUE;
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
		RES_LOGOFF								//�������߰�*/
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
	case RES_KEY2:
		return new CMake_KEY2(src_packet);
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


}

CMake_Start::~CMake_Start()
{
	
}

int CMake_Start::GetPacket(u_char** packet)
{
	if (this->m_packet)
	{
		*packet = this->m_packet;
		return TRUE;
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
		//���Ʊ���mac�������ķ��Ͱ�
		memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);
		
		char str[5] = {0};
		sprintf_s(str, 5, "%d", namelen+5);
		//��ֵ ���� ��
		memcpy_s(this->m_packet + 17, len, str, 5);
		memcpy_s(this->m_packet + 21, len, str, 5);

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
		return TRUE;
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
	//���Ʊ���mac�������ķ��Ͱ�
	memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);

	char str[5] = { 0 };
	sprintf_s(str, 5, "%d", namelen + 22);
	//��ֵ ���� ��
	memcpy_s(this->m_packet + 17, len, str, 5);
	memcpy_s(this->m_packet + 21, len, str, 5);

	//��ֵ id ��
	memcpy_s(this->m_packet + 19, len, &src_packet[19], 1);

	//��ֵ �˺� ��
	memcpy_s(this->m_packet + 40, len, name, namelen);

	/////////////////////////////////////////////////////////////////////
	//����MD5
	u_char MD5Res[16];
	MD5_CTX context;
	BYTE	msgbuf[128] = { 0 };
	int i = 0, j = 0;
	char* pwd = CUI::GetuserPWD();
	
	//������MD5-Challenge�İ�ID
	msgbuf[i++] = src_packet[19];

	//�����ǵ�½����
	while (j< strlen(pwd))
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
	//��ֵMD5��
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
		return TRUE;
	}
	return FALSE;
}