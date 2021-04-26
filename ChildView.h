// PlayDemoDlg.h : header file
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DlgOpenFile.h"
#include "HoverButton.h"
#include "NiceSlider.h"
#include "DisplayWnd.h"
#include "PlayStateText.h"
#include "DlgPicSetting.h"
#include "DlgSetColor.h"
#include "CutFileDlg.h"
#include "DecodeEngineDlg.h"
#include "MediaInfoDlg.h"
#include "Player.h"
#include "EasySize.h"
#include "BitMapSlider.h"
#include "afxwin.h"
#include "PlayDemoDlg.h"

enum MENU_STATE{FILEOPEN, FILECLOSE, FILEINDEXCREATED, STOPPLAY, STARTPLAY,ENABLEFISHEYE,DISABLEFISHEYE};
// 枚举编码格式
enum EncodingType {
	ENCODINGTYPE_ANSI = 0,    // ANSI
	ENCODINGTYPE_ULE,         // UCS Little Endian
	ENCODINGTYPE_UBE,         // UCS Big Endian
	ENCODINGTYPE_UTF8,        // UTF-8
	ENCODINGTYPE_UTF8_NOBOM,  // UTF-8 No BOM
};
const int c_nListWidth     = 140;

/////////////////////////////////////////////////////////////////////////////
// CChildView dialog

class CChildView : public CFormView
{
	DECLARE_DYNCREATE(CChildView)
	// Construction
public:
	CChildView();	// standard constructor
	virtual ~CChildView();

	DECLARE_EASYSIZE

	// Dialog Data
	//{{AFX_DATA(CChildView)
	enum { IDD = IDD_PLAYDEMO_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
public:
	BOOL CreateView(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext);

	// Implementation
protected:
	HICON m_hIcon;
	HACCEL m_hotKey;
	BOOL   m_bInitUpdate;
	// Generated message map functions
	//{{AFX_MSG(CChildView)
	//virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnHelpAbout();

	// menu list
	//回放
	afx_msg void OnFileOpen();
	afx_msg void OnFileClose();
	//系统设置
	afx_msg void OnSettingSnappic();
	afx_msg void OnLocate();	
	afx_msg void OnCutFile();
	afx_msg void OnMediaInfo();
	afx_msg void OnVerticalSync();
	afx_msg void OnIVS();	
	afx_msg void OnSettingCololr();

	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderAudio(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawList2(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnSettingPrimodialsize();
	afx_msg LRESULT OnEnctypeChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSrcArea(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIndexCreated(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFisheyeDetect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnOperator(UINT uID);
	afx_msg void OnButtonAudio();
	afx_msg void OnButtonPicture();
	//afx_msg void OnButtonSetcolor();
	afx_msg void OnButtonBackword();
	afx_msg void OnButtonForword();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnButtonStop();
	afx_msg void OnBnClickedButtonFull();
	afx_msg void OnBnClickedButtonPlaylist();
	afx_msg void OnDevCase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void OpenFile(CString csFilePath);
	void AdaptWindowSize( UINT cxScreen );
	int ChangeUIState(PLAY_STATE nState);
	int ChangeSingleUIState(PLAY_STATE nState, BOOL bEnable);
	void ChangePlayButtonUIState(CHoverButton* lpUI[STATE_SIZE] , int nState[9]);
	int ChangeMenuState(MENU_STATE nState);
	int MediaInit();
	int OpenAudio();
	int CloseAudio();
	int AddFileList(CString csFilePath);
	int	SetCurIndex(int iIndex);
	CMenu* GetMenu();
private:

	CNiceSliderCtrl m_sdProc;
	CNiceSliderCtrl	m_sdAudioWave;
	CNiceSliderCtrl	m_sdAuidoVolume;


	CHoverButton	m_bnPlay;
	CHoverButton	m_bnPause;
	CHoverButton	m_bnStop;
	CHoverButton	m_bnToEnd;
	CHoverButton	m_bnToBegin;
	CHoverButton	m_bnBackOne;
	CHoverButton	m_bnStepOne;
	CHoverButton	m_bnSlow;
	CHoverButton	m_bnFast;
	CHoverButton	m_bnPicCatch;
	//CHoverButton	m_bnSetColor;
	CHoverButton	m_bnAudio;
	CHoverButton	m_bnBackword;
	CHoverButton	m_bnForword;
	CHoverButton	m_bnPlayList;
	CHoverButton	m_bnFull;

	CListCtrl       m_listCtrl;
	CDlgSetColor m_dlgSetColor;


	CDlgOpenFile m_dlgOpenFile;


	CDisplayWnd m_dlgDisplay;


	CPlayStateText m_dlgStateText;


	CDlgPicSetting m_dlgPicSetting;

	DecodeEngineDlg m_dlgDecodeEngine;

	PLAY_STATE m_lastState;

	UINT m_lastFisheyeMode;
	CToolTipCtrl m_ContentTip;
	
	CMenu*	    m_pMenu;

	int			m_nCurSelectItem;
public:
	afx_msg void OnFisheyeviewWall1pplus8();
	afx_msg void OnFisheyeviewFloor1pplus6();
	afx_msg void OnFisheyeviewFloor1plus2();
	afx_msg void OnFisheyeviewCeil1pplus1();
	afx_msg void OnFisheyeviewCeil1plus3();
	afx_msg void OnFisheyeviewFloor2p();
	afx_msg void OnSettingStartFisheye();
	afx_msg void OnSettingDecodeEngine();
	int m_nMoveFrontDistance;
	int m_nMoveBackDistance;
	int m_nMoveLeftDistance;
	int m_nMoveRightDistance;
	int m_nMoveTopDistance;
	int m_nMoveBottomDistance;
	double m_dTotalDistanceZ;
	double m_dTotalDistanceX;
	double m_dTotalDistanceY;

	double m_nLastTime;
	double m_nNowTime;

	int m_nThrowMode;
private:
	int m_DisplayAngle;
	FILE* m_fTranslate;
public:
	void ChangeAngle(int type);
	afx_msg void OnRotateAngle(UINT ID);
	afx_msg void OnSetIvsEnable(UINT ID);
	afx_msg void OnSettingThrowframe();
	afx_msg void OnFisheyeviewSemi();
	afx_msg void OnFisheyeviewCylinder();
	afx_msg void OnFisheyeviewLittle();
	afx_msg void OnFisheyeview360Sphere();
	afx_msg void OnFisheyeview360Cylinder();
	afx_msg void OnAntiAliasing();

	afx_msg void OnSemiCeil();
	afx_msg void OnSemiFloor();
	afx_msg void OnSemiWall();
	afx_msg void OnCylinderCeil();
	afx_msg void OnCylinderFloor();
	afx_msg void OnCylinderWall();
	afx_msg void OnLittleCeil();
	afx_msg void OnLittleFloor();
	afx_msg void OnLittleWall();
	afx_msg void OnSetThrowMode(UINT ID);
private:
	BOOL CheckUTF8NoBOM(void* pBuffer, long size);
	EncodingType GetEncodingTypeFromStr(char *src, long len);
};
