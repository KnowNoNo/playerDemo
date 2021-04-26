// PlayDemo.h : main header file for the PLAYDEMO application
//

#pragma once


#include "resource.h"		// main symbols
#include "afxwinappex.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayDemoApp:
// See PlayDemo.cpp for the implementation of this class
//

class CPlayDemoApp : public CWinAppEx
{
public:
	CPlayDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayDemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// й╣ож
public:
	CMultiDocTemplate *m_pPlay;
	CMultiDocTemplate *m_pAnalysis;
public:
	void OnFileNew();
public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPlayDemoApp theApp;
/////////////////////////////////////////////////////////////////////////////

