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
	//}}AFX_VIRTUAL

// й╣ож

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

