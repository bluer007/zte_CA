#pragma once

class CRever
{
public:
	CRever(const char* AdapterName);
	~CRever();
	
	int StartReve(const u_char** captured, int* len);
	
private:
	//char* adapter_description;
	char* adapter_name;

	//���ݴ���������������,���ظ�����������name,
	//�ɹ�����0	,name��������������ʧ�ܷ��ر�ʾnameӦ���еĴ�С  ,�������󷵻�-1
	//int FindAdapter(const char* description, char* name, int namelen);

	int GetMAC(char* mac);		//����adapter_description����������mac��ַ��mac��������,�ɹ�����TRUE,ʧ�ܷ���FALSE
};