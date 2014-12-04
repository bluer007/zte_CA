#pragma once
#define WIN32
#include "pcap.h" 
#include <Windows.h>
#include <tchar.h>


#include "resource.h"
#pragma comment(lib,"wpcap.lib")

//error
#define		NO_WINPAP				1		//����Ѱ������ʧ��,Winpap�����밲װ�������°�װWinpap		
#define		NO_Description			2		//���棺����û��һ����Ч������
#define		NO_Adapter				3		//���󣺿���û������������������װWinpap



//success
#define		OK_Init					101		//��ʼ���ɹ����ɹ�����������




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

