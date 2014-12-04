#include "Public.h"
#include "UI.h"

CUI::CUI()
{

}

CUI::~CUI()
{

}

int CUI::OutError(int code)		//���������Ϣ
{
	return 0;
}

int CUI::InitUI()
{
	::CoInitialize(NULL);
	CPaintManagerUI::SetInstance(((HINSTANCE)::GetModuleHandle(NULL)));
	//CPaintManagerUI::SetResourceZip(_T("duilib.zip"));
	
	//CFrameWnd *pFrame = new CFrameWnd(_T("duilib.xml"));
	this->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	this->ShowModal();

	
	::CoUninitialize();
	return 0;
}

UILIB_RESOURCETYPE CUI::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}//


LPCTSTR CUI::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);	//IDR_ZIPRES1
}//

CDuiString CUI::GetSkinFile()
{
	return _T("duilib.xml");
}//

void CUI::InitWindow()
{
	//    SetIcon(IDR_MAINFRAME); // ����������ͼ��
	CenterWindow();


}

void CUI::Notify(TNotifyUI& msg)
{

	__super::Notify(msg);
}

CDuiString CUI::GetSkinFolder()
{
	return _T("");
}
