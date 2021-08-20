// PlayDemo.h : main header file for the PLAYDEMO application
//

#pragma once

#include "resource.h"		// main symbols
#include "afxwinappex.h"
#include "CarInfo.h"
#include "map"
#include "./Oracle/DbOperation.h"
/////////////////////////////////////////////////////////////////////////////
// CPlayDemoApp:
// See PlayDemo.cpp for the implementation of this class
//
enum DateSelectType
{
	YEAR,
	MONTH,
	DAY,
	HOUR,
};

class CAnalysisInfo
{
public:
	int					nCarCount;
	CTime				tmStart;					// 数据限制起始时间
	CTime				tmEnd;						// 数据限制终止时间
	CString				strDbFilePath;
	std::map<CString,CCarInfo*>	mpCarInfo;			// 机车号-机车信息
	CWinThread			*pLoadDataThread;
	CRITICAL_SECTION	section;

	CAnalysisInfo()
	{
		InitializeCriticalSection(&section);
		nCarCount = 0;
		pLoadDataThread = NULL;

		CString path; 
		GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
		path.ReleaseBuffer(); 
		int pos = path.ReverseFind('\\'); 
		path = path.Left(pos); 
		strDbFilePath = path;
		strDbFilePath += L"\\sql.udl";
	}
	~CAnalysisInfo()
	{
		DeleteCriticalSection(&section);
		Clear();
	}
	void Lock(void)
	{
		EnterCriticalSection(&section);
	}
	void Unlock(void)
	{
		LeaveCriticalSection(&section);
	}

	BOOL LoadData()
	{
		CString dbFilePath = strDbFilePath;

		CDbOperation dbOperator;

		//	如果CDbOperation在线程中，则必须在此线程中CoInitialize(NULL)
		CoInitialize(NULL);

		Clear();
		//	获取数据库数据

		if( !dbOperator.GetAllData(dbFilePath,tmStart,tmEnd))
		{
			return FALSE;
		}

		CString carName,picPath,videoPath;
		COleDateTime date;
		//	分析数据库数据
		while (!dbOperator.RecordSet.IsEOF())
		{
			carName = dbOperator.RecordSet.GetValueString(L"CAR_NAME");
			if(carName.GetLength() < 1)
				continue;
			date    = dbOperator.RecordSet.GetValueDate(L"HAPPEN_DATE");
			if(date.GetStatus() != 0)
				continue;
			picPath = dbOperator.RecordSet.GetValueString(L"PIC_PATH");
			videoPath = dbOperator.RecordSet.GetValueString(L"VIDEO_PATH");

			if(mpCarInfo.count(carName) < 1)
			{
				mpCarInfo.insert(std::map<CString,CCarInfo*>::value_type(carName,new CCarInfo));
			}
			mpCarInfo[carName]->UpdateData(date.GetYear(),
				date.GetMonth(),date.GetDay(),date.GetHour(),picPath,videoPath);
			dbOperator.RecordSet.MoveNext();
		}

		dbOperator.RecordSet.Close();
		return TRUE;
	}

	void Clear()
	{
		for(std::map<CString,CCarInfo*>::iterator it = mpCarInfo.begin() ; it!=mpCarInfo.end() ;it++)
		{
			delete it->second;
		}
		mpCarInfo.clear();
	}
	//void BeginLoadDataThread(LPVOID lpVoid)
	//{
	//	if(pLoadDataThread)		// 若线程已经运行，则等待结束
	//	{
	//		::WaitForSingleObject(pLoadDataThread->m_hThread,0);
	//		delete pLoadDataThread;
	//		pLoadDataThread=NULL;
	//	}

	//	if(pLoadDataThread = AfxBeginThread(ThreadProc_LoadData,lpVoid,THREAD_PRIORITY_ABOVE_NORMAL,
	//		0,CREATE_SUSPENDED,NULL))
	//	{
	//		pLoadDataThread->m_bAutoDelete=FALSE;
	//		pLoadDataThread->ResumeThread();
	//	}
	//	return;
	//}

	int GetData(CString strCarId,int nYear = 0, int nMonth = 0, int nDay = 0, int nHour = 0)
	{
		if(mpCarInfo.count(strCarId) < 1)
		{
			TRACE(L"查询数据 未出现 %s 机车",strCarId);
			return 0;
		}
		return mpCarInfo[strCarId]->GetData(nYear,nMonth,nDay,nHour);
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

// 实现
public:
	CMultiDocTemplate	*m_pPlay;
	CMultiDocTemplate	*m_pAnalysis;
	CMultiDocTemplate   *m_pElements;

	CAnalysisInfo		*m_pAnalyPathInfo;
	CString				m_strAppPath;
	CString				m_strDbFilePath;
	CString				m_strPicPath;
	CString				m_strVideoPath;
	CString				m_strIP;
	int					m_nPort;
public:
	void LoadData();
	void OnFileNew();
	BOOL InitConfigure(void);
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

