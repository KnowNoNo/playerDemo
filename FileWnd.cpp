
#include "stdafx.h"
#include "mainfrm.h"
#include "FileWnd.h"
#include "PlayDemo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileWnd

CFileWnd::CFileWnd()
{
}

CFileWnd::~CFileWnd()
{
}

BEGIN_MESSAGE_MAP(CFileWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	//CRect rectDummy;
	//rectDummy.SetRectEmpty();

	//// ������ͼ:
	//const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE /*| TVS_HASLINES*/ | TVS_LINESATROOT | TVS_HASBUTTONS;

	//if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	//{
	//	TRACE0("δ�ܴ����ļ���ͼ\n");
	//	return -1;      // δ�ܴ���
	//}

	//// ������ͼͼ��:
	//m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	//m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	//m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	//OnChangeVisualStyle();

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	//m_wndToolBar.SetOwner(this);

	//// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//// ��������ͼ����
	//FillFileView();
	//AdjustLayout();

	CRect rect;
	GetClientRect(&rect);

	m_FileView.Create(NULL, L"", WS_VISIBLE|WS_CHILD, rect, this, 0, NULL); 
	AdjustLayout();

	return 0;
}

void CFileWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileWnd::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("��������"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("5000"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("5000"), 2, 2, hSrc);
	m_wndFileView.InsertItem(_T("5000"), 2, 2, hSrc);
	m_wndFileView.InsertItem(_T("5000"), 2, 2, hSrc);
	m_wndFileView.InsertItem(_T("5000"), 2, 2, hSrc);
	m_wndFileView.InsertItem(_T("5000"), 2, 2, hSrc);
	m_wndFileView.InsertItem(_T("5000"), 2, 2, hSrc);

	HTREEITEM hInc = m_wndFileView.InsertItem(_T("5001"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("5001"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("5001"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("5001"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("5001"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("5001"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("5001"), 2, 2, hInc);

	HTREEITEM hRes = m_wndFileView.InsertItem(_T("5002"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("5002"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("5002"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("5002"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("5002"), 2, 2, hRes);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hInc, TVE_EXPAND);
}

void CFileWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	if(::IsWindow(m_FileView.m_hWnd))
		m_FileView.MoveWindow(rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height());  

	//int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	//m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileWnd::OnProperties()
{
	//AfxMessageBox(_T("����...."));

}

void CFileWnd::OnFileOpen()
{
	// TODO: �ڴ˴���������������
}

void CFileWnd::OnFileOpenWith()
{
	// TODO: �ڴ˴���������������
}

void CFileWnd::OnDummyCompile()
{
	// TODO: �ڴ˴���������������
}

void CFileWnd::OnEditCut()
{
	// TODO: �ڴ˴���������������
}

void CFileWnd::OnEditCopy()
{
	// TODO: �ڴ˴���������������
}

void CFileWnd::OnEditClear()
{
	// TODO: �ڴ˴���������������
}

void CFileWnd::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	//m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	//m_wndFileView.SetFocus();
}

void CFileWnd::OnChangeVisualStyle()
{
	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ���� */);

	//m_FileViewImages.DeleteImageList();

	//UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	//CBitmap bmp;
	//if (!bmp.LoadBitmap(uiBmpId))
	//{
	//	TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
	//	ASSERT(FALSE);
	//	return;
	//}

	//BITMAP bmpObj;
	//bmp.GetBitmap(&bmpObj);

	//UINT nFlags = ILC_MASK;

	//nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	//m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	//m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	//m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


