#pragma once

class CRever
{
public:
	CRever(const char* AdapterName);
	~CRever();
	
	int StartReve(const u_char** captured, int* len);
	static int GetMAC(char* mac);		//根据adapter_name将该网卡的mac地址到mac变量里面,成功返回TRUE,失败返回FALSE

private:
	//char* adapter_description;
	static char* adapter_name;

	//根据传进来的网卡描述,返回该网卡的名字name,
	//成功返回0	,name缓冲区不够大则失败返回表示name应该有的大小  ,其他错误返回-1
	//int FindAdapter(const char* description, char* name, int namelen);

	
};