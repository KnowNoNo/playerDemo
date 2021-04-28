// PlayDemo.h : main header file for the PLAYDEMO application
//

#pragma once


#include "resource.h"		// main symbols
#include "afxwinappex.h"
#include "map"
/////////////////////////////////////////////////////////////////////////////
// CPlayDemoApp:
// See PlayDemo.cpp for the implementation of this class
//
class CUnitInfo
{
public:
	std::map<int,int[12][31]>	mpInfo;
protected:
	CRITICAL_SECTION	section;
public:
	CUnitInfo()
	{
		InitializeCriticalSection(&section);
	}
	~CUnitInfo()
	{
		DeleteCriticalSection(&section);
	}
	void Lock(void)
	{
		EnterCriticalSection(&section);
	}
	void Unlock(void)
	{
		LeaveCriticalSection(&section);
	}

};

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

