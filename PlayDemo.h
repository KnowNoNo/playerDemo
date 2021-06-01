// PlayDemo.h : main header file for the PLAYDEMO application
//

#pragma once

#include "resource.h"		// main symbols
#include "afxwinappex.h"
#include "map"

#define		INIT_STOREINFO_DATETIME			CTime(2021,1,1,0,0,0)
//#define		MAX_CAR_COUNT					50
#define		MAX_CAR_YEAR_COUNT				50
#define		MAX_CAR_MONTH_COUNT				12
#define		MAX_CAR_DAY_COUNT				31

//	������ ��ʻ������Ϣ ��
class CDateInfo
{
public:
	int		nCount;		// �ļ�����
	CString csPath;		// �ļ�λ��
	CDateInfo()
	{
		nCount = 0;
		csPath = L"";
	}
};
//	���� ��ʻ������Ϣ ��
class CCarInfo
{
public:
	int			nID;													// ����ID / ip
	int			nDefaultInitYear;										// Ĭ�����ݳ�ʼ�� �������
	CDateInfo	arrInfo[MAX_CAR_YEAR_COUNT + 1][MAX_CAR_MONTH_COUNT + 1][MAX_CAR_DAY_COUNT + 1];
	
	CCarInfo()
	{
		nID = -1;
		nDefaultInitYear = (INIT_STOREINFO_DATETIME.GetYear()) - 1;
	}

	int GetCount(int nYear = 0, int nMonth = 0, int nDay = 0)			// ��ø������� ��ʻ������Ϣ���� �������б�nY,nM,nD��= nY��nM��nD��  |  ���ֵ=0,��2021,2,0����1,2��= 2021��2��,...
	{
		//	ʱ�䲻�ܵ���Ĭ�ϳ�ʼʱ��2021��1��1��		����2021+MAX_CAR_YEAR_COUNT��
		if(nYear <= nDefaultInitYear || nYear > nDefaultInitYear+MAX_CAR_YEAR_COUNT)
			return -1;

		return arrInfo[nYear - nDefaultInitYear][nMonth][nDay].nCount;
	}

	CString GetPath(int nYear = 0, int nMonth = 0, int nDay = 0)		// ��ø������� ��ʻ�����ļ����ļ���λ��
	{
		if(nYear <= nDefaultInitYear || nYear > nDefaultInitYear+MAX_CAR_YEAR_COUNT)
			return L"";

		return arrInfo[nYear - nDefaultInitYear][nMonth][nDay].csPath;
	}

};

//	���ݷ����� 
class CAnalysisInfo
{
public:
	int						nCarCount;		// ��������
	std::map<int,CCarInfo>	carInfo;		// ������Ϣ

protected:
	CRITICAL_SECTION	section;
public:
	CAnalysisInfo()
	{
		InitializeCriticalSection(&section);
	}
	~CAnalysisInfo()
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

// ʵ��
public:
	CMultiDocTemplate	*m_pPlay;
	CMultiDocTemplate	*m_pAnalysis;
	CMultiDocTemplate   *m_pElements;

	CAnalysisInfo		*m_pAnalyInfo;

public:
	void LoadDate();
	void OnFileNew();
public:
	BOOL  m_bInit;
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

