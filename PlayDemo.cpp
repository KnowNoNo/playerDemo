// PlayDemo.cpp : Defines the class behaviors for the application.
//

#include "StdAfx.h"
#include "PlayDemo.h"
#include "PlayDemoDlg.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "PlayDemoDoc.h"
//#include "PlayDemoView.h"

#include "ChildView.h"
#include "AnalysisView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayDemoApp

BEGIN_MESSAGE_MAP(CPlayDemoApp, CWinAppEx)
	//{{AFX_MSG_MAP(CPlayDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_APP_ABOUT, &CPlayDemoApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CPlayDemoApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayDemoApp construction

CPlayDemoApp::CPlayDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bHiColorIcons = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPlayDemoApp object

CPlayDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlayDemoApp initialization

BOOL CPlayDemoApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象


	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	m_pPlay = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CPlayDemoDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CChildView));
	if (!m_pPlay)
		return FALSE;
	AddDocTemplate(m_pPlay);
		
	m_pAnalysis = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CPlayDemoDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CAnalysisView));
	if (!m_pAnalysis)
		return FALSE;
	AddDocTemplate(m_pAnalysis);
	////
	// 若要创建主窗口，此代码将创建新的框架窗口
	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生


	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	m_pMainWnd->SetWindowText(L"乘务员疲劳检测分析系统V1.0");
	return TRUE;
}

int CPlayDemoApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	BCGCBProCleanUp();
	return CWinAppEx::ExitInstance();
}

///////////////

class CAboutDlg1 : public CDialog
{
public:
	CAboutDlg1();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg1::CAboutDlg1() : CDialog(CAboutDlg1::IDD)
{
}

void CAboutDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg1, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CPlayDemoApp::OnAppAbout()
{
	CAboutDlg1 aboutDlg;
	aboutDlg.DoModal();
}


// CPlayDemoApp 自定义加载/保存方法

void CPlayDemoApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CPlayDemoApp::LoadCustomState()
{
}

void CPlayDemoApp::SaveCustomState()
{
}

void CPlayDemoApp::OnFileNew()
{
	m_pPlay->OpenDocumentFile(NULL);
	m_pAnalysis->OpenDocumentFile(NULL);
}

// CPlayDemoApp 消息处理程序
