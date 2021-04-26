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
	// �����ļ��ı�׼�ĵ�����
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
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
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

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���


	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	m_pPlay = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CPlayDemoDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CChildView));
	if (!m_pPlay)
		return FALSE;
	AddDocTemplate(m_pPlay);
		
	m_pAnalysis = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CPlayDemoDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CAnalysisView));
	if (!m_pAnalysis)
		return FALSE;
	AddDocTemplate(m_pAnalysis);
	////
	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������


	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	m_pMainWnd->SetWindowText(L"����Աƣ�ͼ�����ϵͳV1.0");
	return TRUE;
}

int CPlayDemoApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	BCGCBProCleanUp();
	return CWinAppEx::ExitInstance();
}

///////////////

class CAboutDlg1 : public CDialog
{
public:
	CAboutDlg1();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CPlayDemoApp::OnAppAbout()
{
	CAboutDlg1 aboutDlg;
	aboutDlg.DoModal();
}


// CPlayDemoApp �Զ������/���淽��

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

// CPlayDemoApp ��Ϣ�������
