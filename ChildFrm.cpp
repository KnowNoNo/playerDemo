
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "PlayDemo.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_CHILDACTIVATE()
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{	
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	
	cs.style &= ~WS_HSCROLL;
	cs.style &= ~WS_VSCROLL;
	
	cs.style &= ~FWS_ADDTOTITLE;

	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序

void CChildFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
	// TODO: 在此添加专用代码和/或调用基类

	CMDIChildWndEx::OnUpdateFrameMenu(bActive, pActiveWnd, hMenuAlt);

}


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CChildFrame::OnChildActivate()
{
	CMDIChildWndEx::OnChildActivate();
	
	// TODO: 在此处添加消息处理程序代码

	SendMessage(WM_ERASEBKGND,(WPARAM)GetDC()->GetSafeHdc(),0);
}
