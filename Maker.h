#pragma once
#include "Public.h"

//�򵥹���ģʽ--��Ʒ����ģ��
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


//�򵥹���ģʽ--����ģ��
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

//���������İ�
RES_START,								//�������߰�
RES_ID,									//����ID��ݰ�
RES_MD5,								//����MD5��Ӧ��
RES_KEY1,								//��������KEY1��
RES_KEY2,								//��������KEY2��
RES_LOGOFF								//�������߰�

*/

//�򵥹���ģʽ--��Ʒʵ��ģ��
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
