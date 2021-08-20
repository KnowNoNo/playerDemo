#pragma once
#include "StdAfx.h"
#include "resource.h"
#include "map"
#include "PlayDemo.h"
// CFileView ������ͼ

#define DEFAULT_FTP_UPLOAD_TIMES				5
#define DEFAULT_FTP_SERVER_PIC_FILE_DIR			L"/pic"
#define DEFAULT_FTP_SERVER_VIDEO_FILE_DIR		L"/video"


class CCarFileInfo;
class FileDownLoadInfo
{
public:
	CString		csTPFileName;						// �����ļ���   Ҳ��MAP_ALL_UPLOADFILE_INFO map�ṹ���е�KEYֵ
	ULONGLONG	ullTPFileByte;						// �����ļ���С

	CString		csLocFileDir;
	CString		csServFileDir;
	CString		csServIp;
	int			nServPort;
	CString		csServUser;
	CString		csServPassword;
	bool		bIsPort;
	bool		bIsForce;
	UINT        uIsTpOk;
	bool		bTPMode;							// 0 Ϊ�ϴ�  1 Ϊ����
	CWinThread* pUploadThread;						// �����߳�
	HANDLE		hEvtEndModule;						// ģ������¼�
	ULONGLONG   ullTPDestnFileSize ;				// ����Ŀ�ĵ�����ͬ���ļ��ֽ�
	volatile ULONGLONG	ullTpFileSize;				// ʵʱ��¼�����ֽ���
	volatile ULONGLONG  ullTPFileLastSize;			// ��¼ǰһ��Ĵ����ֽ���������ͳ�������ٶ�
	
	CString	    strCarId;
	CString		strTm;
	CString     strLocFilePath;
	int			nLocFileType;
	CCarFileInfo *pCarInfo;

	FileDownLoadInfo()
	{
		csServIp   = theApp.m_strIP;
		nServPort  = theApp.m_nPort;
		
		csServUser = L"anonymous";
		csServPassword = L"";
		bIsPort  = false;
		bIsForce = false;
		bTPMode  = true;	
		uIsTpOk  = 0;	
		ullTPDestnFileSize= 0;
		ullTpFileSize	  = 0;
		ullTPFileByte     = 0; 
		ullTPFileLastSize = 0;
		hEvtEndModule     = ::CreateEvent ( NULL, TRUE, FALSE, NULL );
		pUploadThread     = NULL; 

		pCarInfo = 0;
	}
	~FileDownLoadInfo()
	{
		CloseHandle(hEvtEndModule);
	}

};

//	�����ļ��߳�
UINT ThreadProc_DownLoadFile(LPVOID lParam);
UINT ThreadProc_LoadCarId(LPVOID lParam);

class CFilePath
{
public:
	CString strServPicPath;
	CString strServVideoPath;

	CString strLocPicPath;
	CString strLocVideoPath;
	CFilePath()
	{
		strLocPicPath = L"";
		strLocVideoPath = L"";
		strServPicPath = L"";
		strServVideoPath = L"";
	}
};

class CFileInfo
{
public:
	std::map<CString,CFilePath*> mpFilePath;		// key - ʱ�� (%Y-%m-%d %H:%M:%S)  value - �ļ�·��

	CFileInfo()
	{
		mpFilePath.clear();
	}

	~CFileInfo()
	{
		Clear();
	}

	void Clear()
	{
		for(std::map<CString,CFilePath*>::iterator it = mpFilePath.begin() ; it!=mpFilePath.end() ;it++)
		{
			delete it->second;
		}
		mpFilePath.clear();
	}

	void AddDatetime(CString strTm)
	{
		if(mpFilePath.count(strTm) < 1)
		{
			mpFilePath.insert(std::map<CString,CFilePath*>::value_type(strTm,new CFilePath));
		}
	}
	void UpdateServFile(CString strTm,CString strPicPath,CString strVideoPath)
	{
		AddDatetime(strTm);

		mpFilePath[strTm]->strServPicPath = strPicPath;
		mpFilePath[strTm]->strServVideoPath = strVideoPath;

	}

	void UpdateLocFile(CString strTm,CString strFilePath,int nFileType = 0)
	{
		AddDatetime(strTm);

		if(nFileType == 0)
			mpFilePath[strTm]->strLocPicPath = strFilePath;
		else
			mpFilePath[strTm]->strServVideoPath = strFilePath;
	}



};

class CCarFileInfo
{
public:
	std::map<CString,CFileInfo*>	m_mpFileInfo;		// key - ������  value - �����ļ���Ϣ	
	CRITICAL_SECTION	section;
	
	CCarFileInfo()
	{
		InitializeCriticalSection(&section);
		Clear();
		init();
	}
	~CCarFileInfo()
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
	void init()
	{
		//AfxBeginThread(ThreadProc_LoadCarId,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,0,0,NULL);
	}
	void Clear()
	{
		for(std::map<CString,CFileInfo*>::iterator it = m_mpFileInfo.begin() ; it!=m_mpFileInfo.end() ;it++)
		{
			delete it->second;
		}
		m_mpFileInfo.clear();
	}
	
	void AddCarId(CString strCarId)
	{
		if(m_mpFileInfo.count(strCarId) < 1)
		{
			m_mpFileInfo.insert(std::map<CString,CFileInfo*>::value_type(strCarId,new CFileInfo));
		}
	}

	void AddFileInfo(CString strCarId,COleDateTime tmFile,CString strPicPath,CString strVideoPath)
	{
		CString strTm;
		strTm = tmFile.Format(L"%Y-%m-%d %H:%M:%S");

		UpdateServFileInfo(strCarId,strTm,strPicPath,strVideoPath);
		
		//		
		FileDownLoadInfo	*info = new FileDownLoadInfo;
		info->strCarId = strCarId;
		info->strTm	  = strTm;
		
		info->csServFileDir = DEFAULT_FTP_SERVER_PIC_FILE_DIR;
		info->csTPFileName  = GetFileNameFormPath(strPicPath);
		info->csLocFileDir  = theApp.m_strPicPath;
		info->strLocFilePath = theApp.m_strVideoPath + L"\\" + info->csTPFileName;
		info->nLocFileType = 0;
		info->pCarInfo = this;

		DownLoadFile((LPVOID)info);

		//
		FileDownLoadInfo	*info1 = new FileDownLoadInfo;
		info1->strCarId = strCarId;
		info1->strTm	  = strTm;

		info1->csServFileDir = DEFAULT_FTP_SERVER_VIDEO_FILE_DIR;		
		info1->csTPFileName  = GetFileNameFormPath(strVideoPath);
		info1->csLocFileDir  = theApp.m_strVideoPath;
		info1->strLocFilePath = theApp.m_strVideoPath + L"\\" + info1->csTPFileName;
		info1->nLocFileType   = 1;
		info1->pCarInfo = this;

		DownLoadFile((LPVOID)info1);

	}
		
	//	���±���ͼƬ·�� nFileType = 0 ����ͼƬ
	void UpdateLocFileInfo(CString strCarId,CString strTm,CString strLocFilePath,int nFileType = 0)
	{
		AddCarId(strCarId);

		m_mpFileInfo[strCarId]->UpdateLocFile(strTm,strLocFilePath,nFileType);
	}

	//	���·�����ͼƬ·�� 
	void UpdateServFileInfo(CString strCarId,CString strTm,CString strServPicFilePath,CString strServVideoFilePath)
	{
		AddCarId(strCarId);

		m_mpFileInfo[strCarId]->UpdateServFile(strTm,strServPicFilePath,strServVideoFilePath);
	}

	void DownLoadFile(LPVOID lparam)
	{

		AfxBeginThread(ThreadProc_DownLoadFile,lparam,THREAD_PRIORITY_ABOVE_NORMAL,0,0,NULL);
	}

	std::map<CString,CFilePath*>::iterator GetFileIter(CString strCarId, CTime tmAsk,BOOL bLowerBound = TRUE)
	{
		CString strTm;
		strTm = tmAsk.Format(L"%Y-%m-%d %H:%M:%S");
		if(bLowerBound)
		{
			return m_mpFileInfo[strCarId]->mpFilePath.lower_bound(strTm);
		}
		else
		{
			return m_mpFileInfo[strCarId]->mpFilePath.upper_bound(strTm);
		}

	}
	
};

class CFileView : public CFormView
{
	DECLARE_DYNCREATE(CFileView)

public:
	CFileView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CFileView();

public:
	enum { IDD = IDD_FORMVIEW_FILE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void PostNcDestroy(); 
	virtual void OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy(); 
	DECLARE_MESSAGE_MAP()

	virtual void OnInitialUpdate();

public:
	CComboBox		m_ComboxCarSelect;
	CComboBox		m_ComboxPlaySource;
	std::map<CString,BOOL>	m_mpCarSelect;

	CDateTimeCtrl   m_DateStart;
	CDateTimeCtrl   m_TimeStart;
	CDateTimeCtrl   m_DateEnd;
	CDateTimeCtrl   m_TimeEnd;

	CCarFileInfo	*m_pCarFileInfo;
	CWinThread		*m_pLoadDataThread;
	CWinThread		*m_pLoadLocFileThread;

public:
	BOOL GetDateTime(CTime &tmR,int nDateTimeType = 0);
	BOOL TimeCheck();
	void BeginLoadDataThread();
	void BeginLoadLocFileThread();
	void BeginInitComboxCarCtrl();
	
	void PlayBack();
	void ChangeDateTimeUI(BOOL bEnable);
	CView* GetChildView();
	void InitComboxCarSelCtrl();
	BOOL FindLocFile(CString strFilePath,BOOL bIsVideoFile = TRUE);

public:
	afx_msg void OnBnClickedButton2();
};


