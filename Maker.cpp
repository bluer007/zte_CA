#include "Maker.h"
#include "Reve.h"
#include "UI.h"
#include "md5.h"
#include "rc4.h"

////////////////////////////简单工厂模式--产品抽象模块
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


/////////////////////////////////////简单工厂模式--工厂模块
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
	//本机发出的包
	RES_START,				//本机上线包
		RES_ID,				//本机ID身份包
		RES_MD5,				//本机MD5回应包
		RES_KEY1,				//本机续网KEY1包
		RES_KEY2,				//本机续网KEY2包
		RES_LOGOFF				//本机下线包
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


//简单工厂模式--产品实体模块
//////////////////////////////////////////////////////////////////////////
CMake_Start::CMake_Start(const u_char* src_packet)
{
	//const u_char BroadcastAddr[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // 广播MAC地址
	//const u_char MultcastAddr[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x03 }; // 多播MAC地址
	this->m_packet = new u_char[18]
	{
		0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,			//多播MAC地址(802.1x规定)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//本机mac地址
		0x88, 0x8e,									//802.1x协议
		0x01,										//802.1x版本
		0x01,										//表示start
		0x00, 0x00									//长度(0)
	};
	this->m_packetLen = 18;

	u_char my_mac[8] = {0};
	assert(CRever::GetMAC(my_mac));
	//赋值本机mac
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
//制造ID身份包
CMake_ID::CMake_ID(const u_char* src_packet)
{
	if (src_packet)
	{
		const char* name = CUI::Getusername();
		const int namelen = strlen(name);
		const int len = 23 + namelen;		//23表下面已知的项的字节数
		this->m_packet = new u_char[len]
		{
			0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,			//对方MAC
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//自己MAC
			0x88, 0x8e,									//网络协议类型(802.1x)
			0x01,										//Version: 1
			0x00,										//Type: EAP Packet (0)
			0x00, 0x00,									//长度，十六进制表示下一项开始到数据包结束所占长度
			0x02,										//Code: Response (2)
			0x00,										//Id: 由发来的包决定
			0x00, 0x00,									//Length:  (同上面的长度项)
			0x01,										//Type: Identity [RFC3748] (1)
			//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //存放自己的账号，是ascii码，我校账号长度为11
			//(现根据Getusername()返回的账号得账号长度)
		};
		this->m_packetLen = len;

		//复制本机mac到即将的发送包
		memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);
		
		char str[1] = { namelen + 5 };
		
		//赋值 长度 项
		memcpy_s(this->m_packet + 17, len, str, 1);
		memcpy_s(this->m_packet + 21, len, str, 1);

		//赋值 id 项
		memcpy_s(this->m_packet + 19, len, &src_packet[19], 1);

		//赋值 账号 项
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
//制造MD5挑战包(即发送加密密码)
CMake_MD5::CMake_MD5(const u_char* src_packet)
{
	const char* name = CUI::Getusername();
	const int namelen = strlen(name);
	const int len = 40 + namelen;		//40表下面已知的项的字节数
	this->m_packet = new u_char[len]
	{
		0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,			//对方MAC
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//自己MAC
		0x88, 0x8e,								//类型
		0x01,									//Version: 1
		0x00,									//Type: EAP Packet (0)
		0x00, 0x00,								//长度，表从下面一项开始到最后所占字节数
		0x02,									//Code: Response (2)
		0x00,									//Id: 跟发来的md5挑战请求包一样
		0x00, 0x00,								//Length: 同上面的长度一样
		0x04,									//Type: MD5-Challenge [RFC3748] (4)
		0x10,									//Value-Size: 16
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//16字节的MD5的前8字节
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//16字节的MD5的后8字节
		//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //存放自己的账号，是ascii码，我校账号长度为11
	};
	this->m_packetLen = len;

	//复制本机mac到即将的发送包
	memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);

	char str[1] = { namelen + 22 };
	
	//赋值 长度 项
	memcpy_s(this->m_packet + 17, len, str, 1);
	memcpy_s(this->m_packet + 21, len, str, 1);

	//赋值 id 项
	memcpy_s(this->m_packet + 19, len, &src_packet[19], 1);

	//赋值 账号 项
	memcpy_s(this->m_packet + 40, len, name, namelen);

	//生成MD5
	u_char MD5Res[16];
	MD5_CTX context;
	BYTE	msgbuf[128] = { 0 };
	int i = 0, j = 0;
	char* pwd = CUI::GetuserPWD();
	
	//以下是MD5-Challenge的包ID
	msgbuf[i++] = src_packet[19];

	//以下是登陆密码
	while (j< (int)strlen(pwd))
	{
		msgbuf[i++] = pwd[j++];
	}

	//服务器发来的MD5-Challenge
	memcpy_s(msgbuf + i, 128, &src_packet[24], 16);
	i += 16;

	//取得MD5信息摘要，存放在MD5Res中
	MD5Init(&context);
	MD5Update(&context, msgbuf, i);
	MD5Final(MD5Res, &context);
	//MD5-Challenge:
	//赋值MD5项(其中md5项为(id+密码+md5请求challenge)计算出来的md5)
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
//生成续网key1包
CMake_KEY1::CMake_KEY1(const u_char* src_packet)
{
	const int len = 78;
	this->m_packet = new u_char[len]
	{
			0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,						//对方MAC
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,						//自己MAC
			0x88, 0x8e,												//协议类型
			0x01,													//Version: 1
			0x03,													//Type: EAP Packet (3)
			0x00, 0x3c,												//长度，3c代表10进制的60
			0x01,													//Descriptor Type: RC4 Descriptor (1)
			0x00, 0x10,												//key length,表示十进制的16
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//Replay Counter: 8字节
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16字节的Key IV 前8字节
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16字节的Key IV 后8字节
			0x00,													//index	
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16字节的Key Signature 前8字节
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16字节的Key Signature 后8字节
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//16字节的Key 前8字节
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00			//16字节的Key 后8字节
	};
	this->m_packetLen = len;

	//复制本机mac到即将的发送包
	memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);

	//赋值 Replay Counter 和  Key IV 项,原样复制，共24字节
	memcpy_s(this->m_packet + 21, len, src_packet + 21, 24);

	//赋值index项
	memcpy_s(this->m_packet + 45, len, src_packet + 45, 1);

	
	u_char enckey[] = 
	{	//海大中兴的加密因子(不同学校可能不同)
		0x02, 0x0E, 0x05, 0x04, 0x66, 0x40, 0x19, 0x75
		, 0x06, 0x06, 0x00, 0x16, 0xD3, 0xF3, 0xAC, 0x02		//luzj加密因子
		/*0x02, 0x0e, 0x05, 0x04, 0x18, 0x0f, 0x02, 0x0a,
		0x06, 0x06, 0x04, 0x7d, 0x7b, 0x98, 0x17, 0xc1*/
	};
	u_char wholekey[20];
	memcpy_s(wholekey, 20, src_packet + 29, 16);		//src_packet + 29表Key IV的全部16个字节
	memcpy_s(wholekey + 16, 20, src_packet + 41, 4);		//src_packet + 41表Key IV最后的4个字节
	int keylen = 16;
	struct rc4_state s;
	rc4_setup(&s, wholekey, 20);
	rc4_crypt(&s, enckey, keylen);
	//赋值key项(由Key IV+Key IV最后4个字节,进行rc4算法所得)
	memcpy_s(&this->m_packet[62], len, enckey, 16);

	//使用hmac_md5算法生成Key Signature，此用于包的校验
	u_char deckey[64] = { 0 };
	u_char encDat[64] = {0};
	//enckey[0] = src_packet[45];	//++++
	memcpy_s(encDat, 64, this->m_packet + 14, 64);		//src_packet + 14表从Version项开始到结束共64字节

	printf("\n\n encDat____\n");
	for (int i = 1; i <=64;i++)
	{
		if ((i % 16) == 1)
			printf("\n");
		printf("%02x ", encDat[i-1]);
	}	printf("\n\n deckey-----\n");


	hmac_md5(encDat, 64, (unsigned char*)&src_packet[45], 1, deckey);		//src_packet[45]表index项

	for (int i = 1; i <= 64; i++)
	{
		if ((i % 16) == 1)
			printf("\n");
		printf("%02x ", deckey[i-1]);
	}printf("\n\n");


	//赋值Key Signature项 (由version开始到结束为止的包内容(Key Signature填充0)进行hmac—md5运算(密钥是index项)的结果.)
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
//生成logoff下线包
CMake_LOGOFF::CMake_LOGOFF(const u_char* src_packet)
{
	//const u_char BroadcastAddr[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // 广播MAC地址
	//const u_char MultcastAddr[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x03 }; // 多播MAC地址
	this->m_packet = new u_char[18]
	{
		0x01, 0x80, 0xc2, 0x00, 0x00, 0x03,			//多播MAC地址(802.1x规定)
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//本机mac地址
		0x88, 0x8e,									//802.1x协议
		0x01,										//802.1x版本
		0x02,										//表示logoff
		0x00, 0x00									//长度(0)
	};
	this->m_packetLen = 18;

	u_char my_mac[8] = { 0 };
	assert(CRever::GetMAC(my_mac));
	//赋值本机mac
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