#include "Public.h"
#include "UI.h"

CUI::CUI()
{

}

CUI::~CUI()
{

}

int CUI::OutError(int code)		//输出错误信息
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
	//    SetIcon(IDR_MAINFRAME); // 设置任务栏图标
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
