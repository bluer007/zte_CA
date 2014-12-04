#pragma once

class CRever
{
public:
	CRever(char* adapter,int time,int timeout);
	~CRever();
	
	void IsStop(bool isStop);
	int StartReve(char* captured);
	
private:
	char* adapter_description;
	int m_time;
	int m_timeout;
	bool m_isStop;

	//根据传进来的网卡描述,返回该网卡的名字name,
	//成功返回0	,name缓冲区不够大则失败返回表示name应该有的大小  ,其他错误返回-1
	int FindAdapter(const char* description, char* name, int namelen);
};