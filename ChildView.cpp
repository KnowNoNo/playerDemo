
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "ChildView.h"
#include "PlayDemo.h"
#include "MainFrm.h"
#include <windows.h>
#include "LocateDlg.h"
#include "Player.h"
#include "LanguageConvertor.h"
#include "GetDllVersion.h"
#include "CmdDialog.h"
#include "dhplayEx.h"
#include "CharactorTansfer.h"
#include <string>
#include <Winnls.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "Version.lib")
#pragma comment(lib,"Kernel32.lib")
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CChildView dialog

//#define SUPPORT_FISYEYE_VR 0

IMPLEMENT_DYNCREATE(CChildView, CFormView)

CChildView::CChildView()
:CFormView(CChildView::IDD)
, m_lastFisheyeMode(0)
, m_DisplayAngle(0)

{
	//{{AFX_DATA_INIT(CChildView)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_nThrowMode = ID_THROW_NO;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_fTranslate = NULL;
	m_nCurSelectItem = -1;
	m_bInitUpdate	= FALSE;
	m_pMenu = new CMenu;
	m_pMenu->LoadMenu(IDR_MENU);
}

CChildView::~CChildView()
{
	delete m_pMenu;
}

void CChildView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildView)
	DDX_Control(pDX, IDC_SLIDER_PROC, m_sdProc);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_bnPause);
	//DDX_Control(pDX, IDC_BUTTON_SETCOLOR, m_bnSetColor);
	DDX_Control(pDX, IDC_BUTTON_TOEND, m_bnToEnd);
	DDX_Control(pDX, IDC_BUTTON_TOBEGIN, m_bnToBegin);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_bnStop);
	DDX_Control(pDX, IDC_BUTTON_SLOW, m_bnSlow);
	DDX_Control(pDX, IDC_BUTTON_PICTURE, m_bnPicCatch);
	DDX_Control(pDX, IDC_BUTTON_ONEBYONE, m_bnStepOne);
	DDX_Control(pDX, IDC_BUTTON_BACKONCE, m_bnBackOne);
	DDX_Control(pDX, IDC_BUTTON_FAST, m_bnFast);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_bnPlay);
	DDX_Control(pDX, IDC_SLIDER_WAVE, m_sdAudioWave);
	DDX_Control(pDX, IDC_SLIDER_AUDIO, m_sdAuidoVolume);
	DDX_Control(pDX, IDC_BUTTON_AUDIO, m_bnAudio);
	DDX_Control(pDX, IDC_BUTTON_BACKWORD, m_bnBackword);
	DDX_Control(pDX, IDC_BUTTON_FORWORD, m_bnForword);
	DDX_Control(pDX, IDC_BUTTON_PLAYLIST, m_bnPlayList);
	DDX_Control(pDX, IDC_BUTTON_FULL, m_bnFull);
	DDX_Control(pDX, IDC_LIST2, m_listCtrl);


	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChildView, CFormView)
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_FILE_OPEN, OnFileOpen)
	ON_COMMAND(IDM_FILE_CLOSE, OnFileClose)
	ON_COMMAND(IDM_SETTING_LOCATE, OnLocate)
	ON_COMMAND(IDM_SETTING_CUTFILE, OnCutFile)
	ON_COMMAND(IDM_SETTING_MEDIAINFO, OnMediaInfo)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_COMMAND(IDM_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(IDM_SETTING_SNAPPIC, OnSettingSnappic)
	ON_COMMAND(IDM_SETTING_VERTICALSYNC, OnVerticalSync)
	ON_COMMAND(IDM_SETTING_IVS, OnIVS)
	ON_BN_CLICKED(IDC_BUTTON_PICTURE, OnButtonPicture)
	ON_BN_CLICKED(IDC_BUTTON_AUDIO, OnButtonAudio)
	//ON_BN_CLICKED(IDC_BUTTON_SETCOLOR, OnButtonSetcolor)
	ON_COMMAND_RANGE(IDC_BUTTON_PLAY, IDC_BUTTON_FAST, OnOperator)
	ON_BN_CLICKED(IDC_BUTTON_BACKWORD, OnButtonBackword)
	ON_BN_CLICKED(IDC_BUTTON_FORWORD, OnButtonForword)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_MSD_INDEXCREATED, OnIndexCreated)
	ON_MESSAGE(WM_USER_MSD_FISHEYEDEVICE_DETECT, OnFisheyeDetect)
	ON_COMMAND(ID_FISHEYEVIEW_WALL_1PPLUS8, OnFisheyeviewWall1pplus8)
	ON_COMMAND(ID_FISHEYEVIEW_FLOOR_1PPLUS6, OnFisheyeviewFloor1pplus6)
	ON_COMMAND(ID_FISHEYEVIEW_FLOOR_1PLUS2, OnFisheyeviewFloor1plus2)
	ON_COMMAND(ID_FISHEYEVIEW_CEIL_1PPLUS1, OnFisheyeviewCeil1pplus1)
	ON_COMMAND(ID_FISHEYEVIEW_CEIL_1PLUS3, OnFisheyeviewCeil1plus3)
	ON_COMMAND(ID_FISHEYEVIEW_FLOOR_2P, OnFisheyeviewFloor2p)
	ON_COMMAND(ID_SETTING_START_FISHEYE, OnSettingStartFisheye)
	ON_COMMAND(ID_SETTING_DECODEENGINE, OnSettingDecodeEngine)
	ON_COMMAND_RANGE(ID_ROTATEANGLE_0, ID_ACCELERATOR_F8, OnRotateAngle)
	ON_COMMAND_RANGE(ID_IVSENABLE_RULE, ID_IVSENABLE_CROWDDISTRIHEAT, OnSetIvsEnable)

	ON_COMMAND_RANGE(ID_THROW_NO, ID_THROW_ADAPTION, OnSetThrowMode)

	ON_COMMAND(ID_ACCELERATOR_F9, OnDevCase)
	ON_COMMAND(ID_SETTING_THROWFRAME, OnSettingThrowframe)
	ON_COMMAND(ID_FISHEYEVIEW_SEMI, OnFisheyeviewSemi)
	ON_COMMAND(ID_FISHEYEVIEW_CYLINDER, OnFisheyeviewCylinder)
	ON_COMMAND(ID_FISHEYEVIEW_LITTLE, OnFisheyeviewLittle)
	ON_COMMAND(ID_FISHEYEVIEW_360_SHPERE, OnFisheyeview360Sphere)
	ON_COMMAND(ID_FISHEYEVIEW_360_CYLINDER, OnFisheyeview360Cylinder)
	ON_COMMAND(ID_SETTING_ANTI, OnAntiAliasing)
	ON_COMMAND(ID_SEMI_CEIL, &CChildView::OnSemiCeil)
	ON_COMMAND(ID_SEMI_FLOOR, &CChildView::OnSemiFloor)
	ON_COMMAND(ID_SEMI_WALL, &CChildView::OnSemiWall)
	ON_COMMAND(ID_CYLINDER_CEIL, &CChildView::OnCylinderCeil)
	ON_COMMAND(ID_CYLINDER_FLOOR, &CChildView::OnCylinderFloor)
	ON_COMMAND(ID_CYLINDER_WALL, &CChildView::OnCylinderWall)
	ON_COMMAND(ID_LITTLE_CEIL, &CChildView::OnLittleCeil)
	ON_COMMAND(ID_LITTLE_FLOOR, &CChildView::OnLittleFloor)
	ON_COMMAND(ID_LITTLE_WALL, &CChildView::OnLittleWall)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CChildView::OnNMDblclkList2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_AUDIO, &CChildView::OnNMCustomdrawSliderAudio)
	ON_BN_CLICKED(IDC_BUTTON_FULL, &CChildView::OnBnClickedButtonFull)
	ON_COMMAND(ID_SETTING_COLOLR, &CChildView::OnSettingCololr)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, &CChildView::OnNMCustomdrawList2)
	ON_BN_CLICKED(IDC_BUTTON_PLAYLIST, &CChildView::OnBnClickedButtonPlaylist)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CChildView)

	EASYSIZE(IDC_STATIC_PLAY,ES_BORDER,ES_BORDER,
	ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_PLAY,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_STOP,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_PAUSE,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_TOBEGIN,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_TOEND,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_BACKONCE,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_ONEBYONE,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_SLOW,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_FAST,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_BACKWORD,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_FORWORD,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_PICTURE,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	//EASYSIZE(IDC_BUTTON_SETCOLOR,ES_BORDER,IDC_STATIC_PLAY,
	//ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_AUDIO,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_SLIDER_AUDIO,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_SLIDER_WAVE,ES_BORDER,IDC_STATIC_PLAY,
	ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_SLIDER_PROC,ES_BORDER,IDC_STATIC_PLAY,
	ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_STATIC_MSG,ES_BORDER,IDC_STATIC_PLAY,
	ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_FULL,ES_KEEPSIZE,IDC_STATIC_PLAY,
	ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_BUTTON_PLAYLIST,ES_KEEPSIZE,IDC_STATIC_PLAY,
	ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_LIST2,IDC_STATIC_PLAY,ES_BORDER,
	ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//// 根据分辨率自动调节窗口大小
	//MONITORINFO oMonitor = {};
	//oMonitor.cbSize = sizeof(oMonitor);
	//::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
	//AdaptWindowSize(oMonitor.rcMonitor.right - oMonitor.rcMonitor.left);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChildView::OnSysCommand(UINT nID, LPARAM lParam)
{

	CFormView::OnSysCommand(nID, lParam);
	
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChildView::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CFormView::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChildView::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChildView::OnOperator(UINT uID)
{
	UINT uIDBase = IDC_BUTTON_PLAY;
	/*Button and status enumeration is in sequence relationship which need to be set manually. */
	PLAY_STATE nState = PLAY_STATE(uID - uIDBase);

	/*change player status*/
	CPlayer::Instance()->ChangeState(nState);

	if ( nState == Stop )
	{
		//CMenu* lpMenu = GetMenu();
		//lpMenu->CheckMenuItem(IDM_SETTING_IVS, MF_UNCHECKED);
	}

	/*Operation to switch status*/
	if( CPlayer::Instance()->Do() <= 0)
	{
		ChangeSingleUIState(nState, FALSE);
	}

	/*Refresh status bar*/
	TCHAR* szDesc = CPlayer::Instance()->Description();
	if(szDesc && _tcscmp(szDesc, _T("Play")) == 0) 
	{
		/*Operation of Slow and Fast are special. To switch between slow/fast play, you may need to change play state*/
		nState = Play;
	}
	m_dlgStateText.SetState(szDesc);

	/*change ui status*/
	ChangeUIState(nState);

}

LRESULT CChildView::OnIndexCreated(WPARAM wParam, LPARAM lParam)
{
	CPlayer::Instance()->stateTable[0][5] = wParam;
	if(wParam)
	{
		ChangeUIState(m_lastState);
		ChangeMenuState(FILEINDEXCREATED);
	}
	return 0;
}

LRESULT CChildView::OnFisheyeDetect(WPARAM wParam, LPARAM lParam)
{
	MENU_STATE feMenuState = wParam?ENABLEFISHEYE:DISABLEFISHEYE;
	ChangeMenuState(feMenuState);
	return 0;
}

void CChildView::OnLocate()
{
	CLocateDlg dlg;
	dlg.DoModal();
}

void CChildView::OnCutFile()
{
	CCutFileDlg dlg(m_dlgOpenFile.m_strFile);
	dlg.DoModal();
}

void CChildView::OnMediaInfo()
{
	CMediaInfoDlg dlg;
	dlg.DoModal();
}

void CChildView::OpenFile(CString csFilePath)
{
	/**/
	CPlayer::Instance()->stateTable[0][5] = 0;

	/*Open player*/
	if( CPlayer::Instance()->Open(CPlayer::TYPE(0), 
		csFilePath.GetBuffer(0), 
		m_dlgDisplay.m_hWnd, this->GetSafeHwnd()) <= 0 )
	{
		CPlayer::Instance()->Close();
		AfxMessageBox(LANG_CS("Open file failed!"));
		return;
	}

	/*Change UI status*/
	ChangeUIState(Open);

	/*Initialize status bar, file play be length or total number of frames*/
	m_dlgStateText.SetPlayedTime(0, CPlayer::Instance()->GetTotalTime());
	m_dlgStateText.SetPlayedFrame(0, CPlayer::Instance()->GetTotalFrame());


	MediaInit();

	OnSetThrowMode(m_nThrowMode);

	/*Turn on progress bar timer*/
	SetTimer(1, 1000, NULL);
}

void CChildView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString  str;
	int      iIndex = -1,iOldIndex = -1;
	CString  strFilePath;
	POSITION pos;
	TCHAR    szFilter[]=L"DAV File(*.dav;)|*.dav;|ALL File(*.*)|*.*|";
	CFileDialog fileDlg(TRUE , NULL , NULL , OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT , szFilter ,this);

	if(IDOK == fileDlg.DoModal())
	{	
		pos = fileDlg.GetStartPosition();
		while(pos)
		{
			strFilePath = fileDlg.GetNextPathName(pos);
			AddFileList(strFilePath);
		}
	}
}

void CChildView::OnFileClose() 
{
	// TODO: Add your command handler code here

	/*Reverse operation to Onopenfile*/
	KillTimer(1);

	m_sdProc.SetPos(0);
	m_dlgStateText.Clear();
	m_dlgStateText.Show();

	CloseAudio();
	CPlayer::Instance()->Close();

	//lpMenu->CheckMenuItem(IDM_SETTING_IVS, MF_UNCHECKED);

	//UINT nFlag = lpMenu->GetMenuState(IDM_SETTING_IVS, MF_CHECKED) ;
	//UINT nFlags = MF_BYCOMMAND | (bSetCheck?MF_CHECKED:MF_UNCHECKED);
	//GetMenu()->CheckMenuItem(IDM_SETTING_IVS,  nFlags);

	//CMenu* lpMenu = GetMenu();
	//lpMenu->CheckMenuItem(IDM_SETTING_IVS, MF_BYCOMMAND|MF_UNCHECKED);

	ChangeUIState(Close);

	/*Refresh display window, may still see the last frame*/
	m_dlgDisplay.CloseMultiWnd();
}

int CChildView::ChangeMenuState(MENU_STATE nState)
{
	//	CMenu* lpMenu = GetMenu();
	//	if(nState == FILEOPEN)
	//	{
	//		lpMenu->EnableMenuItem(IDM_FILE_OPEN, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_FILE_CLOSE, MF_ENABLED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_OVERLAY, MF_ENABLED);//only enabled when file is opened and not played
	//
	//#if SUPPORT_FISYEYE_VR
	//		lpMenu->EnableMenuItem(ID_SETTING_START_FISHEYE, MF_ENABLED);
	//		CMenu* pSubMenu = lpMenu->GetSubMenu(1);
	//		pSubMenu->EnableMenuItem(9, MF_BYPOSITION|MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_SHPERE, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_CYLINDER, MF_ENABLED);
	//#endif
	//		lpMenu->EnableMenuItem(IDM_SETTING_VERTICALSYNC, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_SETTING_THROWFRAME, MF_ENABLED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_IVS, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_SETTING_ANTI, MF_ENABLED);
	//		GetDlgItem(IDC_SLIDER_PROC)->EnableWindow(TRUE);
	//
	//	}
	//	else if(nState == FILECLOSE)
	//	{
	//		lpMenu->EnableMenuItem(IDM_FILE_OPEN, MF_ENABLED);
	//		lpMenu->EnableMenuItem(IDM_FILE_CLOSE, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_LOCATE, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_CUTFILE, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_MEDIAINFO, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_OVERLAY, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_VERTICALSYNC, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_IVS, MF_GRAYED);
	//		lpMenu->CheckMenuItem(IDM_SETTING_OVERLAY, MF_UNCHECKED);
	//		lpMenu->CheckMenuItem(IDM_SETTING_VERTICALSYNC, MF_UNCHECKED);
	//		lpMenu->CheckMenuItem(IDM_SETTING_IVS, MF_UNCHECKED);
	//		lpMenu->EnableMenuItem(ID_SETTING_THROWFRAME, MF_GRAYED);
	//		lpMenu->CheckMenuItem(ID_SETTING_THROWFRAME, MF_UNCHECKED);
	//		lpMenu->EnableMenuItem(ID_SETTING_ANTI, MF_GRAYED);
	//		lpMenu->CheckMenuItem(ID_SETTING_ANTI, MF_UNCHECKED);
	//
	//		CMenu* pSubMenu = lpMenu->GetSubMenu(1);
	//		pSubMenu->EnableMenuItem(9, MF_BYPOSITION|MF_GRAYED);
	//		lpMenu->CheckMenuItem(ID_SETTING_START_FISHEYE,  MF_UNCHECKED);
	//		lpMenu->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	//
	//		lpMenu->EnableMenuItem(ID_SETTING_START_FISHEYE, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_CEIL_1PPLUS1, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_CEIL_1PLUS3, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_2P, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_1PLUS2, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_1PPLUS6, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_WALL_1PPLUS8, MF_GRAYED);
	//		
	//		CMenu* pFisheyeModeMenu = pSubMenu->GetSubMenu(9);
	//		pFisheyeModeMenu->EnableMenuItem(6, MF_BYPOSITION|MF_DISABLED);
	//		pFisheyeModeMenu->EnableMenuItem(7, MF_BYPOSITION|MF_DISABLED);
	//		pFisheyeModeMenu->EnableMenuItem(8, MF_BYPOSITION|MF_DISABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_SHPERE, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_CYLINDER, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_SETTING_DECODEENGINE, MF_ENABLED);
	//		
	//		GetDlgItem(IDC_SLIDER_PROC)->EnableWindow(FALSE);
	//	}
	//	/*When index has been created, set single frame back to TRUE, eliminating quick play and slow play.*/
	//	else if(nState == FILEINDEXCREATED)
	//	{
	//		lpMenu->EnableMenuItem(IDM_SETTING_LOCATE, MF_ENABLED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_CUTFILE, MF_ENABLED);
	//	}
	//	else if(nState == STOPPLAY)
	//	{
	//		lpMenu->EnableMenuItem(IDM_SETTING_LOCATE, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_OVERLAY, MF_ENABLED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_VERTICALSYNC, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_SETTING_THROWFRAME, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_IVS, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_SETTING_ANTI, MF_GRAYED);
	//
	//		lpMenu->CheckMenuItem(ID_SETTING_START_FISHEYE, MF_UNCHECKED);
	//		lpMenu->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	//	}
	//	else if(nState == STARTPLAY)
	//	{
	//		if(CPlayer::Instance()->IsIndexCreated())
	//		{
	//			lpMenu->EnableMenuItem(IDM_SETTING_LOCATE, MF_ENABLED);
	//			lpMenu->EnableMenuItem(IDM_SETTING_CUTFILE, MF_ENABLED);
	//		}
	//		lpMenu->EnableMenuItem(IDM_SETTING_OVERLAY, MF_GRAYED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_MEDIAINFO, MF_ENABLED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_VERTICALSYNC, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_SETTING_THROWFRAME, MF_ENABLED);
	//		lpMenu->EnableMenuItem(IDM_SETTING_IVS, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_SETTING_ANTI, MF_ENABLED);
	//	}
	//	else if (nState == ENABLEFISHEYE)
	//	{
	//		CMenu* pSubMenu = lpMenu->GetSubMenu(1);
	//		pSubMenu->EnableMenuItem(9, MF_BYPOSITION|MF_ENABLED);
	//
	//		lpMenu->EnableMenuItem(ID_SETTING_START_FISHEYE, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_CEIL_1PPLUS1, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_CEIL_1PLUS3, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_2P, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_1PLUS2, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_1PPLUS6, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_WALL_1PPLUS8, MF_ENABLED);
	//#if SUPPORT_FISYEYE_VR
	//		CMenu* pFisheyeModeMenu = pSubMenu->GetSubMenu(9);
	//		pFisheyeModeMenu->EnableMenuItem(6, MF_BYPOSITION|MF_ENABLED);
	//		pFisheyeModeMenu->EnableMenuItem(7, MF_BYPOSITION|MF_ENABLED);
	//		pFisheyeModeMenu->EnableMenuItem(8, MF_BYPOSITION|MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_SEMI_CEIL, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_SEMI_FLOOR, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_SEMI_WALL, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_CYLINDER_CEIL, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_CYLINDER_FLOOR, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_CYLINDER_WALL, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_LITTLE_CEIL, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_LITTLE_FLOOR, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_LITTLE_WALL, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_SHPERE, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_CYLINDER, MF_ENABLED);
	//#endif
	//
	//	}
	//	else if (nState == DISABLEFISHEYE)
	//	{
	//#if SUPPORT_FISYEYE_VR
	//		lpMenu->EnableMenuItem(ID_SETTING_START_FISHEYE, MF_ENABLED);
	//		CMenu* pSubMenu = lpMenu->GetSubMenu(1);
	//		pSubMenu->EnableMenuItem(9, MF_BYPOSITION|MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_SHPERE, MF_ENABLED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_CYLINDER, MF_ENABLED);
	//#else
	//		lpMenu->EnableMenuItem(ID_SETTING_START_FISHEYE, MF_GRAYED);
	//		CMenu* pSubMenu = lpMenu->GetSubMenu(1);
	//		pSubMenu->EnableMenuItem(9, MF_BYPOSITION|MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_SHPERE, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_360_CYLINDER, MF_GRAYED);
	//#endif
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_CEIL_1PPLUS1, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_CEIL_1PLUS3, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_2P, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_1PLUS2, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_FLOOR_1PPLUS6, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_FISHEYEVIEW_WALL_1PPLUS8, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_SEMI_CEIL, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_SEMI_FLOOR, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_SEMI_WALL, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_CYLINDER_CEIL, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_CYLINDER_FLOOR, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_CYLINDER_WALL, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_LITTLE_CEIL, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_LITTLE_FLOOR, MF_GRAYED);
	//		lpMenu->EnableMenuItem(ID_LITTLE_WALL, MF_GRAYED);
	//	}

	return 1;
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar)
	{
		CString csTip;
		switch(GetWindowLong(pScrollBar->m_hWnd, GWL_ID))
		{
		case IDC_SLIDER_PROC:
			CPlayer::Instance()->Seek(m_sdProc.GetPos());
			break;
		case IDC_SLIDER_AUDIO:
			if( CPlayer::Instance()->SetAudioVolume(m_sdAuidoVolume.GetPos()) > 0)
			{
				csTip.Format(L"%d",m_sdAuidoVolume.GetPos());
				m_ContentTip.UpdateTipText(csTip,GetDlgItem(IDC_SLIDER_AUDIO));
			}

			break;
		case IDC_SLIDER_WAVE:
			if( CPlayer::Instance()->SetAudioWave(m_sdAudioWave.GetPos()) > 0)
			{
				csTip.Format(L"%d",m_sdAudioWave.GetPos());
				m_ContentTip.UpdateTipText(csTip,GetDlgItem(IDC_SLIDER_WAVE));
			}
			break;
		default:
			break;
		}
	}

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		int nProc = CPlayer::Instance()->GetProcess();
		m_sdProc.SetPos(nProc);

		double dbBitrate;
		CPlayer::Instance()->GetBitrate(&dbBitrate);
		m_dlgStateText.SetBitrate(dbBitrate);

		int nRate;
		CPlayer::Instance()->GetRate(&nRate);
		m_dlgStateText.SetRate(nRate);

		long nWidht  = 0;
		long nHeight = 0;
		CPlayer::Instance()->GetPicSize(&nWidht, &nHeight);
		if (0 != nWidht && 0 != nHeight)
		{
			m_dlgStateText.SetPictureSize(nWidht, nHeight);
		}

		int nPlayTime = CPlayer::Instance()->GetPlayedTime();
		m_dlgStateText.SetPlayedTime(CPlayer::Instance()->GetPlayedTime(), CPlayer::Instance()->GetTotalTime());
		m_dlgStateText.SetPlayedFrame(CPlayer::Instance()->GetPlayedFrame(), CPlayer::Instance()->GetTotalFrame());

		CString csTipText;
		csTipText.Format(L"%02d:%02d:%02d",nPlayTime/3600,nPlayTime/60,nPlayTime%60);
		m_ContentTip.UpdateTipText(csTipText,GetDlgItem(IDC_SLIDER_PROC));
		/*Refresh status bar*/
		m_dlgStateText.Show();
	}

	CFormView::OnTimer(nIDEvent);
}

void CChildView::OnClose() 
{
	/* To close, send a close info. */

	OnFileClose();

	CFormView::OnClose();
}

void CChildView::OnHelpAbout() 
{
	// TODO: Add your command handler code here

	/*CAboutDlg can be CChildView member, but MFC template did not define  CAboutDlg*/
	//CAboutDlg().DoModal();
}

void CChildView::OnSettingSnappic() 
{
	// TODO: Add your command handler code here
	int nLastType = m_dlgPicSetting.m_nType;
	CString lastFilePath = m_dlgPicSetting.m_strPath;
	if (IDOK != m_dlgPicSetting.DoModal())
	{
		m_dlgPicSetting.m_nType = nLastType;
		m_dlgPicSetting.m_strPath = lastFilePath;
	}
}

void CChildView::OnButtonPicture() 
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	const TCHAR* tszExt[] = {_T("bmp"), _T("jpg")};
	CString strFileName;
	strFileName.Format(_T("%s\\%d_%d_%d_%d_%d_%d.%s"), 
		m_dlgPicSetting.m_strPath, 
		localTime.wYear,
		localTime.wMonth,
		localTime.wDay,
		localTime.wHour,
		localTime.wMinute,
		localTime.wSecond,
		tszExt[m_dlgPicSetting.m_nType]);
	if(strFileName == "")
		AfxMessageBox(LANG_CS("picture path error!"));
	TCHAR szShortPath[1024] = {0};
	std::string strShortPath;
	DWORD nLength = GetShortPathNameW(m_dlgPicSetting.m_strPath.GetBuffer(0), szShortPath, sizeof(szShortPath)-1);
	if (nLength == 0)
	{
		strShortPath = UnicodeToAnsi(m_dlgPicSetting.m_strPath.GetBuffer(0));
	}
	else
	{
		strShortPath = UnicodeToAnsi(szShortPath);
	}

	char szShortFileName[2048] = {0};
	const char* szExt[] = {"bmp", "jpg"};
	_snprintf(szShortFileName, sizeof(szShortFileName)-1, "%s\\%d_%d_%d_%d_%d_%d.%s", 
		strShortPath.c_str(), 
		localTime.wYear,
		localTime.wMonth,
		localTime.wDay,
		localTime.wHour,
		localTime.wMinute,
		localTime.wSecond,
		szExt[m_dlgPicSetting.m_nType]);
	if(!CPlayer::Instance()->SnapPicture(szShortFileName, m_dlgPicSetting.m_nType))
		AfxMessageBox(LANG_CS("Snap failed!"));
}

void CChildView::OnVerticalSync()
{
	UINT nCheckFlag = GetMenu()->GetMenuState(IDM_SETTING_VERTICALSYNC, MF_CHECKED) & MF_CHECKED;
	bool bSetCheck = nCheckFlag>0?FALSE:TRUE;
	UINT nFlags = MF_BYCOMMAND | (bSetCheck?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_SETTING_VERTICALSYNC,  nFlags);

	CPlayer::Instance()->SetVerticalSync(bSetCheck);
}


// 检测是否为UTF-8无BOM格式编码
// src为文本内容，len为文本的长度
BOOL CChildView::CheckUTF8NoBOM(void* pBuffer, long size)
{     
	bool IsUTF8 = true;     
	unsigned char* start = (unsigned char*)pBuffer;     
	unsigned char* end = (unsigned char*)pBuffer + size;     
	while (start < end)     
	{     
		if (*start < 0x80) {   
			// (10000000): 值小于0x80的为ASCII字符  
			start++;     
		} else if (*start < (0xC0)) {
			// (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
			IsUTF8 = false;     
			break;     
		} else if (*start < (0xE0)) {     
			// (11100000): 此范围内为2字节UTF-8字符
			if (start >= end - 1) break;     
			if ((start[1] & (0xC0)) != 0x80) {     
				IsUTF8 = false;     
				break;     
			}     
			start += 2;     
		} else if (*start < (0xF0)) {
			// (11110000): 此范围内为3字节UTF-8字符
			if (start >= end - 2) break;     
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80) {     
				IsUTF8 = false;     
				break;     
			}     
			start += 3;     
		} else {     
			IsUTF8 = false;     
			break;     
		}     
	}     
	return IsUTF8;     
}

// 从文本中获取编码格式
// src为文本内容，len为文本的长度
EncodingType CChildView::GetEncodingTypeFromStr(char *src, long len)
{
	PBYTE pBuffer = (PBYTE)src;
	if (pBuffer[0] == 0xFF && pBuffer[1] == 0xFE)
		return ENCODINGTYPE_ULE;
	if (pBuffer[0] == 0xFE && pBuffer[1] == 0xFF)
		return ENCODINGTYPE_UBE;
	if (pBuffer[0] == 0xEF && pBuffer[1] == 0xBB && pBuffer[2] == 0xBF)
		return ENCODINGTYPE_UTF8;
	if (CheckUTF8NoBOM(src, len))
		return ENCODINGTYPE_UTF8_NOBOM;
	else return ENCODINGTYPE_ANSI;
}

void CChildView::OnIVS()
{
	UINT nCheckFlag = GetMenu()->GetMenuState(IDM_SETTING_IVS, MF_CHECKED) & MF_CHECKED;
	BOOL bSetCheck = nCheckFlag>0?FALSE:TRUE;
	UINT nFlags = MF_BYCOMMAND | (bSetCheck?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_SETTING_IVS,  nFlags);

	CPlayer::Instance()->RenderPrivateData(bSetCheck);
	if (bSetCheck)
	{
		GetMenu()->CheckMenuItem(ID_IVSENABLE_RULE, MF_CHECKED);
		GetMenu()->CheckMenuItem(ID_IVSENABLE_TRACK, MF_CHECKED);
		GetMenu()->CheckMenuItem(ID_IVSENABLE_POSEVENT, MF_CHECKED);
		GetMenu()->CheckMenuItem(ID_IVSENABLE_CROWDDISTRIHEAT, MF_CHECKED);
		if(!m_fTranslate)
		{
			//获取语言环境
			LANGID lid = GetSystemDefaultLangID();
			if(lid == 0X0804)
			{
				m_fTranslate = fopen("./SimpChinese.txt","rb");
			}
			else if (lid == 0x0409)
			{
				m_fTranslate = fopen("./English.txt","rb");
			}

			if (m_fTranslate)
			{
				fseek(m_fTranslate, 0, SEEK_END);  
				long lSize = ftell(m_fTranslate);  
				fseek(m_fTranslate, 0, SEEK_SET);  //或rewind(f);  

				char *pBuff = new char[lSize + 1]; 
				if(pBuff == NULL)
				{
					return;
				}
				memset(pBuff, 0, lSize + 1);  
				fread(pBuff, lSize, 1, m_fTranslate);  


				EncodingType  nEncodeType = GetEncodingTypeFromStr(pBuff,lSize);
				if (nEncodeType == ENCODINGTYPE_UTF8)
				{
					CPlayer::Instance()->SetTranslateString(pBuff + 3);
				}
				else if (nEncodeType == ENCODINGTYPE_UTF8_NOBOM)
				{
					CPlayer::Instance()->SetTranslateString(pBuff);
				}
				if (pBuff)
				{
					delete[] pBuff;
					pBuff = NULL;
				}
				fclose(m_fTranslate);  
				m_fTranslate = NULL;

			}	
		}
	}
	else
	{
		GetMenu()->CheckMenuItem(ID_IVSENABLE_RULE, MF_UNCHECKED);
		GetMenu()->CheckMenuItem(ID_IVSENABLE_TRACK, MF_UNCHECKED);
		GetMenu()->CheckMenuItem(ID_IVSENABLE_POSEVENT, MF_UNCHECKED);
		GetMenu()->CheckMenuItem(ID_IVSENABLE_CROWDDISTRIHEAT, MF_UNCHECKED);
	}
}

BOOL CChildView::PreTranslateMessage(MSG* pMsg) 
{
	float SPEED      = 0.5f;

	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_ESCAPE && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
		return 1;
	else if (pMsg->wParam == VK_RETURN && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
		return 1;
	else if(::TranslateAccelerator(GetSafeHwnd(), m_hotKey, pMsg))
		return 1;
	else if (pMsg->wParam == 87 && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		if (m_nLastTime <= 0.005)
		{
			m_nNowTime = ((double)GetTickCount()) / 1000;
			m_nLastTime = m_nNowTime;
		}
		else
		{
			m_nNowTime =((double) GetTickCount()) / 1000;
			if (m_nNowTime < (m_nLastTime + 0.005))
			{
				return 1;
			}
			double nDistance = (m_nNowTime - m_nLastTime) *SPEED;
			m_nLastTime = m_nNowTime;
			PLAY_GetDoubleRegion(0,0, RENDER_STEREO_EYE_MOVE_BACK, &m_dTotalDistanceZ);

			m_dTotalDistanceZ -= (double)nDistance;
			PLAY_SetStereoEyeMoveDistance(0, 0, STEREO_EYE_MOVE_BACK, m_dTotalDistanceZ);
		}
	}
	//s
	else if (pMsg->wParam == 83 && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		if (m_nLastTime <= 0.005)
		{
			m_nNowTime = ((double)GetTickCount()) / 1000;
			m_nLastTime = m_nNowTime;
		}
		else
		{
			m_nNowTime =((double) GetTickCount()) / 1000;

			if (m_nNowTime < (m_nLastTime + 0.005))
			{
				return 1;
			}
			double nDistance = (m_nNowTime - m_nLastTime) *SPEED;
			m_nLastTime = m_nNowTime;
			PLAY_GetDoubleRegion(0,0, RENDER_STEREO_EYE_MOVE_BACK, &m_dTotalDistanceZ);
			m_dTotalDistanceZ += (double)nDistance;

			PLAY_SetStereoEyeMoveDistance(0, 0, STEREO_EYE_MOVE_BACK, m_dTotalDistanceZ);
		}
	}
	//A
	else if (pMsg->wParam == 65 && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		if (m_nLastTime <= 0.005)
		{
			m_nNowTime = ((double)GetTickCount()) / 1000;
			m_nLastTime = m_nNowTime;
		}
		else
		{
			m_nNowTime =((double) GetTickCount()) / 1000;

			if (m_nNowTime < (m_nLastTime + 0.005))
			{
				return 1;
			}
			double nDistance = (m_nNowTime - m_nLastTime) *SPEED;
			m_nLastTime = m_nNowTime;
			PLAY_GetDoubleRegion(0,0, RENDER_STEREO_EYE_MOVE_RIGHT, &m_dTotalDistanceX);

			//物体往左相当于相机往右
			m_dTotalDistanceX += (double)nDistance;
			PLAY_SetStereoEyeMoveDistance(0, 0, STEREO_EYE_MOVE_RIGHT, m_dTotalDistanceX);
		}
	}
	//D
	else if (pMsg->wParam == 68 && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		if (m_nLastTime <= 0.005)
		{
			m_nNowTime = ((double)GetTickCount()) / 1000;
			m_nLastTime = m_nNowTime;
		}
		else
		{
			m_nNowTime =((double) GetTickCount()) / 1000;

			if (m_nNowTime < (m_nLastTime + 0.005))
			{
				return 1;
			}
			double nDistance = (m_nNowTime - m_nLastTime) *SPEED;
			m_nLastTime = m_nNowTime;
			PLAY_GetDoubleRegion(0,0, RENDER_STEREO_EYE_MOVE_RIGHT, &m_dTotalDistanceX);

			m_dTotalDistanceX -= (double)nDistance;
			PLAY_SetStereoEyeMoveDistance(0, 0, STEREO_EYE_MOVE_RIGHT, m_dTotalDistanceX);
		}
	}
	else if (pMsg->wParam == VK_UP && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		if (m_nLastTime <= 0.005)
		{
			m_nNowTime = ((double)GetTickCount()) / 1000;
			m_nLastTime = m_nNowTime;
		}
		else
		{
			m_nNowTime =((double) GetTickCount()) / 1000;

			if (m_nNowTime < (m_nLastTime + 0.005))
			{
				return 1;
			}
			double nDistance = (m_nNowTime - m_nLastTime) *SPEED;
			m_nLastTime = m_nNowTime;
			PLAY_GetDoubleRegion(0,0, RENDER_STEREO_EYE_MOVE_TOP, &m_dTotalDistanceY);

			m_dTotalDistanceY -= (double)nDistance;

			PLAY_SetStereoEyeMoveDistance(0, 0, STEREO_EYE_MOVE_TOP, m_dTotalDistanceY);
		}
	}
	else if (pMsg->wParam == VK_DOWN && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		if (m_nLastTime <= 0.005)
		{
			m_nNowTime = ((double)GetTickCount()) / 1000;
			m_nLastTime = m_nNowTime;
		}
		else
		{
			m_nNowTime =((double) GetTickCount()) / 1000;

			if (m_nNowTime < (m_nLastTime + 0.005))
			{
				return 1;
			}
			double nDistance = (m_nNowTime - m_nLastTime) *SPEED;
			m_nLastTime = m_nNowTime;
			PLAY_GetDoubleRegion(0,0, RENDER_STEREO_EYE_MOVE_TOP, &m_dTotalDistanceY);

			m_dTotalDistanceY += (double)nDistance;

			PLAY_SetStereoEyeMoveDistance(0, 0, STEREO_EYE_MOVE_TOP, m_dTotalDistanceY);
		}
	}
	//o
	else if (pMsg->wParam == 79 && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		double dFovy = 0.0;
		PLAY_GetDoubleRegion(0,0, RENDER_STEREO_PERSPECTIVE_FOVY, &dFovy);

		dFovy -= (double)2;
		if(dFovy > 10)
		{
			PLAY_SetStereoPerspectiveFovy(0, 0, dFovy);
		}
	}
	//i
	else if (pMsg->wParam == 73 && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		double dFovy = 0.0;
		PLAY_GetDoubleRegion(0,0, RENDER_STEREO_PERSPECTIVE_FOVY, &dFovy);

		dFovy += (double)2;
		// TODO: 在此添加控件通知处理程序代码
		if(dFovy < 180)
		{
			PLAY_SetStereoPerspectiveFovy(0, 0, dFovy);
		}
	}
	//p，普通模式观察
	else if (pMsg->wParam == 80 && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		PLAY_SetStereoViewMode(0, 0, STEREO_COMMON_VIEW);
	}
	//n，内视点观察
	else if (pMsg->wParam == 78 && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		PLAY_SetStereoViewMode(0, 0, STEREO_INNER_VIEW);
	}
	else if (pMsg->message == WM_KEYUP || pMsg->message == WM_SYSKEYUP)
	{
		m_nLastTime = 0;
		m_nNowTime = 0;
	}
	else
	{
		if (::IsWindow(m_ContentTip.GetSafeHwnd()))
		{
			m_ContentTip.RelayEvent(pMsg);
		}
		return CFormView::PreTranslateMessage(pMsg);
	}
}

void CChildView::ChangePlayButtonUIState(CHoverButton* lpUI[STATE_SIZE] , int nState[9])
{
	for(int i = 0 ; i < STATE_SIZE; i++)
	{
		if(nState[i]!=2)
		{
			lpUI[i]->EnableWindowEx(nState[i]);
		}
	}
}

//Change UI State
int CChildView::ChangeUIState(PLAY_STATE nState)
{
	CHoverButton* lpUI[STATE_SIZE] = { &m_bnPlay, &m_bnPause, &m_bnStop, &m_bnToEnd, &m_bnToBegin,
		&m_bnBackOne, &m_bnStepOne, &m_bnSlow, &m_bnFast };

	if(nState == Open)
	{
		//m_bnSetColor.EnableWindowEx(TRUE);
		m_bnAudio.EnableWindow(TRUE);
		m_sdAudioWave.EnableWindow(TRUE);
		m_sdAuidoVolume.EnableWindow(TRUE);
		ChangePlayButtonUIState(lpUI, CPlayer::Instance()->openTable);
		ChangeMenuState(FILEOPEN);
	}
	else if(nState == Close)
	{
		m_bnPicCatch.EnableWindowEx(FALSE);
		//m_bnSetColor.EnableWindowEx(FALSE);
		m_bnAudio.EnableWindow(FALSE);
		m_sdAudioWave.EnableWindow(FALSE);
		m_sdAuidoVolume.EnableWindow(FALSE);
		ChangePlayButtonUIState(lpUI, CPlayer::Instance()->closeTable);
		ChangeMenuState(FILECLOSE);
		m_dlgDisplay.Invalidate(TRUE);
	}
	else if(nState>=Play && nState<=Fast)
	{
		m_bnPicCatch.EnableWindowEx(nState != Stop ? TRUE : FALSE);
		if(nState==Stop)
		{
			m_dlgDisplay.Invalidate(TRUE);
			ChangeMenuState(STOPPLAY);
		}

		if (nState == Play)
		{
			ChangeMenuState(STARTPLAY);
		}

		ChangePlayButtonUIState(lpUI, CPlayer::Instance()->stateTable[nState]);
	}

	m_lastState = nState;
	return 1;
}

int CChildView::ChangeSingleUIState(PLAY_STATE nState, BOOL bEnable)
{
	CHoverButton* lpUI[STATE_SIZE] = { &m_bnPlay, &m_bnPause, &m_bnStop,
		&m_bnToEnd, &m_bnToBegin, &m_bnBackOne, 
		&m_bnStepOne, &m_bnSlow, &m_bnFast };
	lpUI[nState]->EnableWindowEx(bEnable);
	return 1;
}

int CChildView::MediaInit()
{
	DecodeType type = (DecodeType)m_dlgDecodeEngine.GetDecodeType();

	if(type == DECODE_HW_FAST_D3D11)
		PLAY_SetEngine(0,type,RENDER_NOTSET);
	else
		PLAY_SetEngine(0,type,RENDER_D3D11);

	ChangeAngle(0);
	/*Audio is on by default.*/
	OpenAudio();
	CPlayer::Instance()->SetEngineType(type);

	return 0;
}

int CChildView::OpenAudio()
{
	CString csTipText;
	CPlayer::Instance()->OpenSound(TRUE);
	m_sdAudioWave.EnableWindow(TRUE);
	m_sdAuidoVolume.EnableWindow(TRUE);

	m_sdAuidoVolume.SetPos(CPlayer::Instance()->GetAudioVolume());
	csTipText.Format(L"%d",CPlayer::Instance()->GetAudioVolume());
	m_ContentTip.UpdateTipText(csTipText,GetDlgItem(IDC_SLIDER_AUDIO));

	CPlayer::Instance()->SetAudioWave(m_sdAudioWave.GetPos());
	csTipText.Format(L"%d",m_sdAudioWave.GetPos());
	m_ContentTip.UpdateTipText(csTipText,GetDlgItem(IDC_SLIDER_WAVE));

	m_bnAudio.EnableWindow(TRUE);

	return 1;
}

int CChildView::CloseAudio()
{
	CPlayer::Instance()->OpenSound(FALSE);
	m_sdAudioWave.EnableWindow(FALSE);
	m_sdAuidoVolume.EnableWindow(FALSE);

	m_bnAudio.EnableWindow(FALSE);
	return 1;
}

void CChildView::OnButtonAudio() 
{
	// TODO: Add your control notification handler code here
	if(m_bnAudio.m_bButtonEnable)
	{
		/*Turn off audio if on*/
		CloseAudio();
	}
	else
	{
		/*Turn on audio if off*/
		OpenAudio();
	}
}

//void CChildView::OnButtonSetcolor() 
//{
//	// TODO: Add your control notification handler code here
//	m_dlgSetColor.DoModal();
//}

void CChildView::OnButtonBackword()
{
	CPlayer::Instance()->SetPlayDirection(1);
}

void CChildView::OnButtonForword()
{
	CPlayer::Instance()->SetPlayDirection(0);
}

void CChildView::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFile[1024] = {0};
	int   nIndex;
	if( DragQueryFile(hDropInfo, 0, szFile, 1024)<=0 )
	{
		return;
	}

	if( (nIndex = AddFileList(szFile)) < 0)
		return;

	OnFileClose();
	m_dlgOpenFile.m_strFile = szFile;
	SetCurIndex(nIndex);

	int pos = -1;
	TCHAR* pbuffer = m_dlgOpenFile.m_strFile.GetBuffer(m_dlgOpenFile.m_strFile.GetLength());
	if ( -1 != (pos = m_dlgOpenFile.m_strFile.ReverseFind('.')))
	{
		pbuffer += pos;
	}

	OpenFile(m_dlgOpenFile.m_strFile);
}

void CChildView::OnSettingStartFisheye()
{
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	UINT nCheckFlag = GetMenu()->GetMenuState(ID_SETTING_START_FISHEYE, MF_CHECKED) & MF_CHECKED;
	BOOL bSetCheck = nCheckFlag>0?FALSE:TRUE;
	UINT nFlags = MF_BYCOMMAND | (bSetCheck?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_SETTING_START_FISHEYE,  nFlags);

	CPlayer::Instance()->ControlFishEye();
}

void CChildView::OnSettingDecodeEngine()
{
	m_dlgDecodeEngine.DoModal();
}

void CChildView::OnFisheyeviewCeil1pplus1()
{
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_CEIL_1PPLUS1,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_CEIL_1PPLUS1;

	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_CEIL, FISHEYECALIBRATE_MODE_PANORAMA_PLUS_ONE_EPTZ);
}

void CChildView::OnFisheyeviewCeil1plus3()
{
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_CEIL_1PLUS3,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_CEIL_1PLUS3;

	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_CEIL, FISHEYECALIBRATE_MODE_ORIGINAL_PLUS_THREE_EPTZ_REGION);
}

void CChildView::OnFisheyeviewFloor2p()
{
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_FLOOR_2P,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_FLOOR_2P;

	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_FLOOR, FISHEYECALIBRATE_MODE_DOUBLE_PANORAMA);
}

void CChildView::OnFisheyeviewFloor1plus2()
{
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_FLOOR_1PLUS2,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_FLOOR_1PLUS2;

	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_FLOOR, FISHEYECALIBRATE_MODE_ORIGINAL_PLUS_TWO_EPTZ_REGION);
}

void CChildView::OnFisheyeviewFloor1pplus6()
{
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_FLOOR_1PPLUS6,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_FLOOR_1PPLUS6;

	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_FLOOR, FISHEYECALIBRATE_MODE_PANORAMA_PLUS_SIX_EPTZ_REGION);
}

void CChildView::OnFisheyeviewWall1pplus8()
{
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_WALL_1PPLUS8,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_WALL_1PPLUS8;

	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_WALL, FISHEYECALIBRATE_MODE_PANORAMA_PLUS_EIGHT_EPTZ_REGION);
}


void CChildView::ChangeAngle(int type)
{
	if(type == m_DisplayAngle)
		return;
	if(type == 4)
		m_DisplayAngle = 0;
	else if(type == -1)
		m_DisplayAngle = 3;
	else
		m_DisplayAngle = type;
	PLAY_SetRotateAngle(0,m_DisplayAngle);
	UINT nFlags;
	nFlags = MF_BYCOMMAND | (m_DisplayAngle == 0?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_ROTATEANGLE_0,  nFlags);
	nFlags = MF_BYCOMMAND | (m_DisplayAngle == 1?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_ROTATEANGLE_90,  nFlags);
	nFlags = MF_BYCOMMAND | (m_DisplayAngle == 2?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_ROTATEANGLE_180,  nFlags);
	nFlags = MF_BYCOMMAND | (m_DisplayAngle == 3?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_ROTATEANGLE_270,  nFlags);
}

void CChildView::OnSetThrowMode(UINT ID)
{
	int nThrowFlags = -1;
	switch(ID)
	{
	case ID_THROW_NO:
		nThrowFlags = 0;
		m_nThrowMode = ID_THROW_NO;
		CPlayer::Instance()->EnableLargePicAdjustment(0); //0为不抽帧
		break;
	case ID_THROW_DEFAULT:
		nThrowFlags = 1;
		m_nThrowMode = ID_THROW_DEFAULT;
		CPlayer::Instance()->EnableLargePicAdjustment(1); // 1为默认策略抽帧
		break;
	case ID_THROW_ADAPTION:
		nThrowFlags = 3;
		m_nThrowMode = ID_THROW_ADAPTION;
		CPlayer::Instance()->EnableLargePicAdjustment(3); //3为自适应抽帧
		break;
	default:
		break;
	}

	if (nThrowFlags != -1)
	{
		int nFlags = 0;
		nFlags = MF_BYCOMMAND | (nThrowFlags == 0?MF_CHECKED:MF_UNCHECKED);
		GetMenu()->CheckMenuItem(ID_THROW_NO,  nFlags);

		nFlags = MF_BYCOMMAND | (nThrowFlags == 1?MF_CHECKED:MF_UNCHECKED);
		GetMenu()->CheckMenuItem(ID_THROW_DEFAULT,  nFlags);

		nFlags = MF_BYCOMMAND | (nThrowFlags == 3?MF_CHECKED:MF_UNCHECKED);
		GetMenu()->CheckMenuItem(ID_THROW_ADAPTION,  nFlags);
	}

	return;
}

void CChildView::OnRotateAngle(UINT ID)
{
	switch(ID)
	{
	case ID_ROTATEANGLE_0:
	case ID_ROTATEANGLE_90:
	case ID_ROTATEANGLE_180:
	case ID_ROTATEANGLE_270:
		ChangeAngle(ID - ID_ROTATEANGLE_0);
		break;
	case ID_ROTATEANGLE_ROTATELEFT:
	case ID_ACCELERATOR_F7:
		ChangeAngle(m_DisplayAngle - 1);
		break;
	case ID_ROTATEANGLE_ROTATERIGHT:
	case ID_ACCELERATOR_F8:
		ChangeAngle(m_DisplayAngle + 1);
		break;
	}
}

void CChildView::OnSetIvsEnable(UINT ID)
{
	UINT nCheckFlag = GetMenu()->GetMenuState(ID, MF_CHECKED) & MF_CHECKED;
	BOOL bSetCheck = nCheckFlag>0?FALSE:TRUE;
	UINT nFlags = MF_BYCOMMAND | (bSetCheck?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID,  nFlags);
	int checkType = 0;
	switch(ID)
	{
	case ID_IVSENABLE_RULE:
		CPlayer::Instance()->SetIvsEnable(IVSDRAWER_RULE, bSetCheck);
		break;
	case ID_IVSENABLE_TRACK:
		CPlayer::Instance()->SetIvsEnable(IVSDRAWER_TRACK, bSetCheck);
		CPlayer::Instance()->SetIvsEnable(IVSDRAWER_TRACKEX2, bSetCheck);
		break;
	case ID_IVSENABLE_POSEVENT:
		CPlayer::Instance()->SetIvsEnable(IVSDRAWER_ALARM, bSetCheck);
		break;
	case ID_IVSENABLE_CROWDDISTRIHEAT:
		CPlayer::Instance()->SetIvsEnable(IVSDRAWER_DATA_WITH_LARGE_AMOUNT, bSetCheck);
		break;
	}
}
void CChildView::OnSettingThrowframe()
{
	UINT nCheckFlag = GetMenu()->GetMenuState(ID_SETTING_THROWFRAME, MF_CHECKED) & MF_CHECKED;
	BOOL bSetCheck = nCheckFlag>0?FALSE:TRUE;
	UINT nFlags = MF_BYCOMMAND | (bSetCheck?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_SETTING_THROWFRAME,  nFlags);

	//0为不抽帧，1为720P以上抽帧
	CPlayer::Instance()->EnableLargePicAdjustment(bSetCheck);
}


void CChildView::OnDevCase()
{
#if (!defined _WIN64) && (defined _DEBUG)
	// 64 地址强转会出现内存问题。暂时不只是win64
	CCmdDialog().DoModal();
#endif

}
void CChildView::OnFisheyeviewSemi()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_SEMI,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_SEMI;

	//CPlayer::Instance()->SetFishEyeMode((FISHEYE_MOUNTMODE)m_lastSemiMount, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_SEMI_SPHERE));

}

void CChildView::OnFisheyeviewCylinder()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_CYLINDER,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_CYLINDER;

	//	CPlayer::Instance()->SetFishEyeMode((FISHEYE_MOUNTMODE)m_lastSemiMount, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_CYLINDER));
}

void CChildView::OnFisheyeviewLittle()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_LITTLE,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_LITTLE;

	//CPlayer::Instance()->SetFishEyeMode((FISHEYE_MOUNTMODE)m_lastSemiMount, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_LITTLE_PLANET));
}

void CChildView::OnFisheyeview360Sphere()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_360_SHPERE,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_360_SHPERE;

	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_CEIL, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_DOUBLE_SPHERE));

}

void CChildView::OnFisheyeview360Cylinder()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_FISHEYEVIEW_360_CYLINDER,  MF_CHECKED);

	m_lastFisheyeMode = ID_FISHEYEVIEW_360_CYLINDER;

	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_CEIL, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_DOUBLE_CYLINDER));
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	static BOOL s_bInit = FALSE;
	if(m_bInitUpdate && theApp.m_bInit)
	{
		s_bInit = TRUE;
		INIT_EASYSIZE;
		m_bInitUpdate = FALSE;
	}
		
	if(s_bInit)	
		UPDATE_EASYSIZE;

	//CFormView::OnSize(nType, cx, cy);

	CWnd* pBoder = GetDlgItem(IDC_STATIC_PLAY);

	if (pBoder && pBoder->GetSafeHwnd() && ::IsWindow(m_dlgDisplay.m_hWnd))
	{
		CRect rBoder;

		pBoder->GetWindowRect(&rBoder);
		pBoder->ScreenToClient(&rBoder);
		m_dlgDisplay.MoveWindow(&rBoder);
	}

} 

void CChildView::OnAntiAliasing()
{
	UINT nCheckFlag = GetMenu()->GetMenuState(ID_SETTING_ANTI, MF_CHECKED) & MF_CHECKED;
	bool bSetCheck = nCheckFlag>0?FALSE:TRUE;
	UINT nFlags = MF_BYCOMMAND | (bSetCheck?MF_CHECKED:MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_SETTING_ANTI,  nFlags);

	CPlayer::Instance()->SetAntiAliasing(bSetCheck);
}

void CChildView::OnSemiCeil()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_SEMI_CEIL,  MF_CHECKED);

	m_lastFisheyeMode = ID_SEMI_CEIL;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_CEIL, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_SEMI_SPHERE));

}

void CChildView::OnSemiFloor()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_SEMI_FLOOR,  MF_CHECKED);

	m_lastFisheyeMode = ID_SEMI_FLOOR;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_FLOOR, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_SEMI_SPHERE));

}

void CChildView::OnSemiWall()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_SEMI_WALL,  MF_CHECKED);

	m_lastFisheyeMode = ID_SEMI_WALL;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_WALL, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_SEMI_SPHERE));
}

void CChildView::OnCylinderCeil()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_CYLINDER_CEIL,  MF_CHECKED);

	m_lastFisheyeMode = ID_CYLINDER_CEIL;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_CEIL, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_CYLINDER));
}

void CChildView::OnCylinderFloor()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_CYLINDER_FLOOR,  MF_CHECKED);

	m_lastFisheyeMode = ID_CYLINDER_FLOOR;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_FLOOR, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_CYLINDER));

}

void CChildView::OnCylinderWall()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_CYLINDER_WALL,  MF_CHECKED);

	m_lastFisheyeMode = ID_CYLINDER_WALL;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_WALL, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_CYLINDER));

}

void CChildView::OnLittleCeil()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_LITTLE_CEIL,  MF_CHECKED);

	m_lastFisheyeMode = ID_LITTLE_CEIL;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_CEIL, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_LITTLE_PLANET));

}

void CChildView::OnLittleFloor()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_LITTLE_FLOOR,  MF_CHECKED);

	m_lastFisheyeMode = ID_LITTLE_FLOOR;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_FLOOR, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_LITTLE_PLANET));

}

void CChildView::OnLittleWall()
{
	// TODO: 在此添加命令处理程序代码
	GetMenu()->CheckMenuItem(m_lastFisheyeMode, MF_UNCHECKED);
	GetMenu()->CheckMenuItem(ID_LITTLE_WALL,  MF_CHECKED);

	m_lastFisheyeMode = ID_LITTLE_WALL;
	CPlayer::Instance()->SetFishEyeMode(FISHEYEMOUNT_MODE_WALL, (FISHEYE_CALIBRATMODE)(FISHEYECALIBRATE_MODE_LITTLE_PLANET));

}

int CChildView::AddFileList(CString csFilePath)
{	
	int			iIndex;
	TCHAR       szName[_MAX_FNAME];
	TCHAR       szExt[_MAX_EXT];
	LVITEM		lvIt;

	if(!PathFileExists(csFilePath))
		return -1;

	_tsplitpath_s(csFilePath.GetBuffer( ), NULL, 0, NULL, 0, szName, _MAX_FNAME, szExt, _MAX_EXT);

	if(_tcscmp(szExt,L".dav") && _tcscmp(szExt,L".avi") && _tcscmp(szExt,L".mp4") )
		return -1;
	iIndex = m_listCtrl.GetItemCount();

	lvIt.mask=LVIF_TEXT;
	lvIt.pszText= szName;
	lvIt.iSubItem=0;
	lvIt.iItem=iIndex;
	iIndex = m_listCtrl.InsertItem(&lvIt);
	ASSERT(iIndex >= 0);

	lvIt.mask=LVIF_TEXT;
	lvIt.iItem=iIndex;
	lvIt.pszText=csFilePath.GetBuffer(0);
	lvIt.iSubItem=1;
	m_listCtrl.SetItem(&lvIt);

	return iIndex;
}
void CChildView::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POSITION iPos  = m_listCtrl.GetFirstSelectedItemPosition();
	int      nItem = m_listCtrl.GetNextSelectedItem(iPos);

	CWnd*p = GetDlgItem(IDC_STATIC_MSG);

	if(nItem < 0 || nItem == m_nCurSelectItem)
		return;

	OnFileClose();
	SetCurIndex(nItem);
	m_dlgOpenFile.m_strFile = m_listCtrl.GetItemText(nItem,1);

	OpenFile(m_listCtrl.GetItemText(nItem,1));
	//m_CSFor_ListCtrl.Lock();
	//OnOperator(IDC_STATIC_PLAY);
	//m_CSFor_ListCtrl.Unlock();
}

void CChildView::OnNMCustomdrawSliderAudio(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CChildView::OnBnClickedButtonFull()
{
	// TODO: 在此添加控件通知处理程序代码
	static BOOL bFull = false;
	bFull^=1;
	if(bFull)
	{
		SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		m_ContentTip.UpdateTipText(L"退出全屏",GetDlgItem(IDC_BUTTON_FULL));
	}
	else
	{
		SendMessage(WM_SYSCOMMAND,SC_RESTORE,0);
		m_ContentTip.UpdateTipText(L"全屏",GetDlgItem(IDC_BUTTON_FULL));
	}


	//ModifyStyle(WS_CAPTION,0,0);
	//int cx,cy; 
	//cx = GetSystemMetrics(SM_CXSCREEN); 
	//cy = GetSystemMetrics(SM_CYSCREEN);

	//CRect rcTemp; 
	//rcTemp.BottomRight() = CPoint(cx, cy); 
	//rcTemp.TopLeft() = CPoint(0, 0); 
	//MoveWindow(&rcTemp);
}

void CChildView::OnSettingCololr()
{
	// TODO: 在此添加命令处理程序代码
	m_dlgSetColor.DoModal();
}

int CChildView::SetCurIndex(int iIndex)
{
	m_nCurSelectItem = iIndex;
	m_listCtrl.Invalidate(TRUE);

	return iIndex;
}
void CChildView::OnNMCustomdrawList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = CDRF_DODEFAULT;

	if(pNMCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if(pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if(pNMCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		COLORREF clrText,clrBk;
		if(pNMCD->nmcd.dwItemSpec == m_nCurSelectItem)
		{
			clrText = RGB(255,0,0);
			//clrBk   = RGB(255,255,255);
		}
		else 
		{
			clrText = RGB(0,0,0);
			//clrBk	= RGB(255,255,255);
		}
		if(pNMCD->nmcd.dwItemSpec & 1)//奇数行
		{
			clrBk   = RGB(255,255,255);
		}
		else
		{
			clrBk   = RGB(240,255,255);
		}
		pNMCD->clrText   = clrText;
		pNMCD->clrTextBk = clrBk;
	}

}

void CChildView::AdaptWindowSize(UINT cxScreen)
{
	int iX = 968, iY = 600;
	int iWidthList = 225, iWidthSearchEdit = 193;
	SIZE szFixSearchBtn = {201, 0};

	if(cxScreen <= 1024)      // 800*600  1024*768  
	{
		iX = 775;
		iY = 470;
	} 
	else if(cxScreen <= 1280) // 1152*864  1280*800  1280*960  1280*1024
	{
		iX = 968;
		iY = 600;
	}
	else if(cxScreen <= 1366) // 1360*768 1366*768
	{
		iX = 1058;
		iY = 656;
		iWidthList        += 21;
		iWidthSearchEdit  += 21;
		szFixSearchBtn.cx += 21;
	}
	else                      // 1440*900
	{
		iX = 1224;
		iY = 760;
		iWidthList        += 66;
		iWidthSearchEdit  += 66;
		szFixSearchBtn.cx += 66;
	}

	if(::IsWindow(this->GetSafeHwnd())) ::SetWindowPos(this->GetSafeHwnd(), NULL, 0, 0, iX, iY, SWP_FRAMECHANGED|SWP_NOZORDER|SWP_NOACTIVATE);
	CenterWindow();
}
BOOL CChildView::CreateView(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							 DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
							 CCreateContext* pContext)
{
	if(Create(lpszClassName, lpszWindowName, dwRequestedStyle, rect,  pParentWnd, nID, pContext))
	{
		
		return TRUE;//this->ShowWindow(SW_SHOW);
	}
	return FALSE;
}

void CChildView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetParent()->SetWindowText(L"视频回放");

	/*load button bmp*/
	m_bnPlay.LoadBitmap(IDB_BITMAP_PLAY);
	m_bnPause.LoadBitmap(IDB_BITMAP_PAUSE);
	m_bnStop.LoadBitmap(IDB_BITMAP_STOP);
	m_bnToEnd.LoadBitmap(IDB_BITMAP_TOEND);
	m_bnBackOne.LoadBitmap(IDB_BITMAP_STEPBACK);
	m_bnStepOne.LoadBitmap(IDB_BITMAP_STEP);
	m_bnSlow.LoadBitmap(IDB_BITMAP_SLOW);
	m_bnFast.LoadBitmap(IDB_BITMAP_FAST);
	m_bnToBegin.LoadBitmap(IDB_BITMAP_TOBEGIN);
	m_bnPicCatch.LoadBitmap(IDB_BITMAP_CAMERA);
	//m_bnPicCatch.LoadBitmap(IDB_BITMAP_SCREENSHOT);
	//m_bnSetColor.LoadBitmap(IDB_BITMAP_SETCOLOR);
	m_bnAudio.LoadBitmap(IDB_BITMAP_SOUND);
	m_bnBackword.LoadBitmap(IDB_BITMAP_STEPBACK);
	m_bnForword.LoadBitmap(IDB_BITMAP_STEP);
	m_bnPlayList.LoadBitmap(IDB_BITMAP_SHOWLIST);
	m_bnFull.LoadBitmap(IDB_BITMAP_FULL);

	m_sdAudioWave.SetRange(-100, 100, true);
	m_sdAudioWave.SetPos(0);
	m_sdAuidoVolume.SetRange(0, 0xffff);

	m_dlgStateText.Init(GetDlgItem(IDC_STATIC_MSG)->GetSafeHwnd());
	//CWnd* pW= GetDlgItem(IDC_STATIC_MSG);

		
	//LIST 
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/ | LVS_EX_HEADERDRAGDROP |LVS_EX_INFOTIP  );
	//m_listCtrl.InsertColumn(0,L"NULL",LVCFMT_LEFT,0);
	m_listCtrl.InsertColumn(0,L"列表",LVCFMT_LEFT,c_nListWidth-50);
	//m_listCtrl.InsertColumn(1,L"时长",LVCFMT_LEFT,c_nListWidth/3-2);
	//m_listCtrl.InsertColumn(3,L"时长（秒）",LVCFMT_LEFT,110);
	m_listCtrl.InsertColumn(1,L"位置",LVCFMT_LEFT,60);

	m_dlgDisplay.Create(IDD_DIALOG_DISPLAY, GetDlgItem(IDC_STATIC_PLAY));
	m_dlgDisplay.ShowWindow(SW_SHOW);
	
	ChangeUIState(Close);

	m_hotKey = ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));

	LANG_INIT();
	LANG_SETWNDSTATICTEXT(this);
	LANG_SETMENUSTATICTEXT(GetMenu());

	//INIT_EASYSIZE;

	if (!m_ContentTip.Create(this, TTS_ALWAYSTIP))
	{
		return;
	}
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_PLAY), _T("播放"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_PAUSE), _T("暂停"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_STOP), _T("停止"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_FAST), _T("加速"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_SLOW), _T("减速"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_BACKWORD), _T("反向播放"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_FORWORD), _T("正向播放"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_BACKONCE), _T("前一帧"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_ONEBYONE), _T("下一帧"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_TOEND), _T("结束"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_TOBEGIN), _T("开头"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_PICTURE),_T("截图"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_AUDIO),_T("音量"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_PLAYLIST),_T("开关播放列表"));
	m_ContentTip.AddTool(GetDlgItem(IDC_BUTTON_FULL),_T("全屏"));
	m_ContentTip.AddTool(GetDlgItem(IDC_SLIDER_AUDIO),_T("00:00"));
	m_ContentTip.AddTool(GetDlgItem(IDC_SLIDER_WAVE),_T("0"));
	m_ContentTip.AddTool(GetDlgItem(IDC_SLIDER_PROC),_T(""));

	m_ContentTip.Activate(TRUE);

	m_bInitUpdate = TRUE;	


	((CMainFrame*)AfxGetMainWnd())->SendMessage(WM_SIZE,0,MAKELPARAM(491,348));
	OnSize(0,491,348);
	return;
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFormView::PreCreateWindow(cs);
}
CMenu* CChildView::GetMenu()
{
	return m_pMenu;
}

void CChildView::OnBnClickedButtonPlaylist()
{
	// TODO: 在此添加控件通知处理程序代码
	static BOOL bShow = TRUE;
	
	CRect rcList,rcClient,rc;
	m_listCtrl.GetWindowRect(&rcList);
	GetClientRect(&rcClient);
	
	rc.SetRect(0,0,rcClient.Width()-(bShow?0:rcList.Width()+3),rcList.Height());
	GetDlgItem(IDC_STATIC_PLAY)->MoveWindow(rc);
	m_dlgDisplay.MoveWindow(&rc);
	//GetDlgItem(IDC_STATIC_PLAY)->SetWindowPos(&wndTop,0,0,rcClient.Width()-(bShow?0:rcList.Width()),0,SWP_NOZORDER|SWP_NOMOVE);
	
	m_listCtrl.ShowWindow(bShow?SW_HIDE:SW_SHOW);
	bShow ^= 1;
}

void CChildView::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	OnFileClose();
}
