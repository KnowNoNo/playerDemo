
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
//#include "ChildView.h"
#include "FileWnd.h"
#include "ClassView.h"
#include "PropertiesWnd.h"
#include "CalendarBar.h"
#include "Resource.h"


class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); }
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

	// 属性
public:

	// 操作
public:

	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileWnd          m_wndFileView;
	//CClassView        m_wndClassView;
	//CPropertiesWnd    m_wndProperties;
	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;
	CCalendarBar      m_wndCalendar;
	
	// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnViewPlay();
	afx_msg void OnViewAnalysis();
	afx_msg void OnViewElements();
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth);
	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);
	void SwitchToView(CDocTemplate * pTemplate, CRuntimeClass * pViewClass);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;

public:
	BOOL		m_bInitPos;
	void		AdaptWindowSize();
	void        GetChildView();


	afx_msg void OnClose();
	afx_msg void OnFileViewOpen();
	afx_msg void OnFileViewClose();
	afx_msg void OnAnalysisViewOpen();
	afx_msg void OnAnalysisViewClose();
	afx_msg void OnAnalysisViewOpen2();
	afx_msg void OnAnalysisViewClose2();
	afx_msg void OnAnalysisViewLoad();
	afx_msg void OnAnalysisViewLoad2();
	afx_msg void OnLoadfileLoc();
};




