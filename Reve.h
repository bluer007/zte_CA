#pragma once


typedef unsigned char u_char;

class CRever
{
public:
	CRever(const char* AdapterName);
	~CRever();
	
	int StartReve(const u_char** captured, int* len);
	static int GetMAC(u_char* mac);		//����adapter_name����������mac��ַ��mac��������,�ɹ�����TRUE,ʧ�ܷ���FALSE

private:
	//char* adapter_description;
	static char* adapter_name;

	//���ݴ���������������,���ظ�����������name,
	//�ɹ�����0	,name��������������ʧ�ܷ��ر�ʾnameӦ���еĴ�С  ,�������󷵻�-1
	//int FindAdapter(const char* description, char* name, int namelen);

	
};