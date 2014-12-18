#pragma once
#include "Public.h"

//简单工厂模式--产品抽象模块
class CPacket
{
public:
	CPacket();
	~CPacket();

	virtual int GetPacket(u_char** packet);

protected:
	u_char* m_packet;
	int m_packetLen;
};


//简单工厂模式--工厂模块
class CMaker
{
public:
	CMaker();
	~CMaker();
	CPacket* MakePacket(const Packet_Type type, const u_char* src_packet, int* IsOK);

private:
	Packet_Type m_type;

};

/*

//本机发出的包
RES_START,								//本机上线包
RES_ID,									//本机ID身份包
RES_MD5,								//本机MD5回应包
RES_KEY1,								//本机续网KEY1包
RES_KEY2,								//本机续网KEY2包
RES_LOGOFF								//本机下线包

*/

//简单工厂模式--产品实体模块
class CMake_Start : public CPacket
{
public:
	CMake_Start(const u_char* src_packet);
	~CMake_Start();
	virtual int GetPacket(u_char** packet);

};

class CMake_ID : public CPacket
{
public:
	CMake_ID(const u_char* src_packet);
	~CMake_ID();
	virtual int GetPacket(u_char** packet);

};

class CMake_MD5 : public CPacket
{
public:
	CMake_MD5(const u_char* src_packet);
	~CMake_MD5();
	virtual int GetPacket(u_char** packet);

};

class CMake_KEY1 : public CPacket
{
public:
	CMake_KEY1(const u_char* src_packet);
	~CMake_KEY1();
	virtual int GetPacket(u_char** packet);

};

class CMake_LOGOFF : public CPacket
{
public:
	CMake_LOGOFF(const u_char* src_packet);
	~CMake_LOGOFF();
	virtual int GetPacket(u_char** packet);

};
