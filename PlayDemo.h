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

//	日期内 驾驶报警信息 类
class CDateInfo
{
public:
	int		nCount;		// 文件数量
	CString csPath;		// 文件位置
	CDateInfo()
	{
		nCount = 0;
		csPath = L"";
	}
};
//	车辆 驾驶报警信息 类
class CCarInfo
{
public:
	int			nID;													// 车辆ID / ip
	int			nDefaultInitYear;										// 默认数据初始化 所在年份
	CDateInfo	arrInfo[MAX_CAR_YEAR_COUNT + 1][MAX_CAR_MONTH_COUNT + 1][MAX_CAR_DAY_COUNT + 1];
	
	CCarInfo()
	{
		nID = -1;
		nDefaultInitYear = (INIT_STOREINFO_DATETIME.GetYear()) - 1;
	}

	int GetCount(int nYear = 0, int nMonth = 0, int nDay = 0)			// 获得该年月日 驾驶报警信息数量 ：参数列表（nY,nM,nD）= nY年nM月nD日  |  如果值=0,（2021,2,0）或（1,2）= 2021年2月,...
	{
		//	时间不能低于默认初始时间2021年1月1日		高于2021+MAX_CAR_YEAR_COUNT年
		if(nYear <= nDefaultInitYear || nYear > nDefaultInitYear+MAX_CAR_YEAR_COUNT)
			return -1;

		return arrInfo[nYear - nDefaultInitYear][nMonth][nDay].nCount;
	}

	CString GetPath(int nYear = 0, int nMonth = 0, int nDay = 0)		// 获得该年月日 驾驶报警文件或文件夹位置
	{
		if(nYear <= nDefaultInitYear || nYear > nDefaultInitYear+MAX_CAR_YEAR_COUNT)
			return L"";

		return arrInfo[nYear - nDefaultInitYear][nMonth][nDay].csPath;
	}

};

//	数据分析类 
class CAnalysisInfo
{
public:
	int						nCarCount;		// 车辆数量
	std::map<int,CCarInfo>	carInfo;		// 车辆信息

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

// 实现
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

