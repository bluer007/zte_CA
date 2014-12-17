#include "Maker.h"
#include "UI.h"
#include "md5.h"

////////////////////////////简单工厂模式--产品抽象模块
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
		RES_LOGOFF								//本机下线包*/
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
		//复制本机mac到即将的发送包
		memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);
		
		char str[5] = {0};
		sprintf_s(str, 5, "%d", namelen+5);
		//赋值 长度 项
		memcpy_s(this->m_packet + 17, len, str, 5);
		memcpy_s(this->m_packet + 21, len, str, 5);

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
		return TRUE;
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
	//复制本机mac到即将的发送包
	memcpy_s(this->m_packet + 6, len - 6, src_packet, 6);

	char str[5] = { 0 };
	sprintf_s(str, 5, "%d", namelen + 22);
	//赋值 长度 项
	memcpy_s(this->m_packet + 17, len, str, 5);
	memcpy_s(this->m_packet + 21, len, str, 5);

	//赋值 id 项
	memcpy_s(this->m_packet + 19, len, &src_packet[19], 1);

	//赋值 账号 项
	memcpy_s(this->m_packet + 40, len, name, namelen);

	/////////////////////////////////////////////////////////////////////
	//生成MD5
	u_char MD5Res[16];
	MD5_CTX context;
	BYTE	msgbuf[128] = { 0 };
	int i = 0, j = 0;
	char* pwd = CUI::GetuserPWD();
	
	//以下是MD5-Challenge的包ID
	msgbuf[i++] = src_packet[19];

	//以下是登陆密码
	while (j< strlen(pwd))
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
	//赋值MD5项
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