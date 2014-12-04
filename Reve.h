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

	//���ݴ���������������,���ظ�����������name,
	//�ɹ�����0	,name��������������ʧ�ܷ��ر�ʾnameӦ���еĴ�С  ,�������󷵻�-1
	int FindAdapter(const char* description, char* name, int namelen);
};