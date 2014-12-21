#pragma once
#define WIN32
#include "pcap.h" 
#include <Windows.h>
#include <tchar.h>

#include "pcap.h"
#include <Iphlpapi.h>		//Ϊ��IP_ADAPTER_INFO�ṹ������ҵ�����,�û���ȡ����mac��ַ
#include "resource.h"
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib,"wpcap.lib")


enum OK_ERROR
{
	//error
	NO_WINPAP				=1	,		//����Ѱ������ʧ��,Winpap�����밲װ�������°�װWinpap		
	NO_Description				,		//���棺����û��һ����Ч������
	NO_Adapter					,		//���󣺿���û������������������װWinpap
	NO_IP						,		//����:��ȡIP��ʱ,����������

	NO_Selecte_Adapter			,		//����:��ѡ����Ч���� ^_^ 
	NO_Username					,		//����:��������Ч�˺��� ^_^
	NO_Pwd						,		//����:��������Ч����
	NO_Online					,		//״̬:����

	//success
	OK_Init					=101,		//��ʼ���ɹ����ɹ�����������
	OK_IP						,		//�ɹ���ȡIP
	OK_Success					,		//�����ɹ�, ����������~~~
	OK_Online							//״̬:����

};


//���ݰ����ͳ���
enum Packet_Type
{
	//�����������İ�
	UN_KNOWN			=1	,				//�����������Ĳ�֪����
	REQ_ID					,				//����������ID��ݰ�
	REQ_MD5					,				//����������MD5��ս��
	REQ_SUCCESS				,				//��������ӦSUCCESS��
	REQ_KEY					,				//����������EAPOL_KEY��
	REQ_FAILURE				,				//��������Ӧfailure��,��������logoff����ȷ��,����,��������ԭ��			

	//���������İ�
	RES_START				,				//�������߰�
	RES_ID					,				//����ID��ݰ�
	RES_MD5					,				//����MD5��Ӧ��
	RES_KEY1				,				//��������KEY1��
	RES_KEY2				,				//��������KEY2��
	RES_LOGOFF								//�������߰�
};



struct INI_RECORD			//ini�ļ��ļ�¼�ṹ
{
	int Timeout;			//��ʱ����
	bool RememberPWD;		//�Ƿ��ס����
	bool Autorun;			//�Ƿ񿪻�����
	bool Autologon;			//�Ƿ��Զ���¼
	int Try;				//�������Ӵ���
	int netcard;			//��ǰѡ�е�����
	int LastUser;			//��ǰ�û��˻�
	char username[100];			//�����û��˺�
	char userPWD[100];			//�����û�����
};

