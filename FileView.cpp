// FileView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlayDemo.h"
#include "FileView.h"
#include "FTPCmdClass.h"
#include "ChildView.h"
//
UINT ThreadProc_LoadFileViewData(LPVOID lParam)
{
	CFileView *pFileView = (CFileView*)lParam;

	//	����ʱ������

	CTime tmStart,tmEnd;
	CString strCarId;

	pFileView->GetDateTime(tmStart,0);
	pFileView->GetDateTime(tmEnd,1);

	pFileView->m_ComboxCarSelect.GetWindowText(strCarId);

	CDbOperation dbOperator;

	//	���CDbOperation���߳��У�������ڴ��߳���CoInitialize(NULL)
	CoInitialize(NULL);

	//	��ȡ���ݿ�����
	if( !dbOperator.GetCarData(theApp.m_strDbFilePath,tmStart,tmEnd,strCarId))
	{
		AfxMessageBox(L"���ݿ����ݻ�ȡʧ��");
		return 0;
	}

	CString carName,picPath,videoPath;
	COleDateTime date;
	//	�������ݿ�����

	pFileView->m_pCarFileInfo->Lock();
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
		
		pFileView->m_pCarFileInfo->AddFileInfo(carName,date,picPath,videoPath);
		
		dbOperator.RecordSet.MoveNext();
	}

	pFileView->m_pCarFileInfo->Unlock();

	pFileView->PlayBack();
	pFileView->ChangeDateTimeUI(TRUE);

	return 0;
}

UINT ThreadProc_LoadlocFile(LPVOID lParam)
{
	CFileView *pFileView = (CFileView*)lParam;

	CString  strLocPicPath = theApp.m_strPicPath + L"\\*.jpg";
	CString  strLocVideoPath = theApp.m_strVideoPath + L"\\*.mp4";
	//	
	pFileView->m_ComboxPlaySource.EnableWindow(FALSE);

	//	��ȡ�����ļ��� �ļ�
	if( !pFileView->FindLocFile(strLocPicPath,FALSE) )
	{
		//AfxMessageBox(theApp.m_strPicPath + L"·�����ļ�Ϊ��!");
	}
	if( !pFileView->FindLocFile(strLocVideoPath,TRUE))
	{
		//AfxMessageBox(theApp.m_strVideoPath + L"·�����ļ�Ϊ��!");
	}
	
	pFileView->m_ComboxPlaySource.EnableWindow(TRUE);
	return 0;
}

//	�����ļ��߳�
UINT ThreadProc_DownLoadFile(LPVOID lParam)
{
	FileDownLoadInfo *pInfo = (FileDownLoadInfo*)lParam;

	CFTPCmdClass *pDownLoadCmd = new CFTPCmdClass;
	CString str;
	ASSERT(pDownLoadCmd);

	BOOL bRet = FALSE;
	for(int i=0 ; i<DEFAULT_FTP_UPLOAD_TIMES ; i++)
	{
		//��¼
		if(!pDownLoadCmd->LogOn(pInfo->csServIp,pInfo->nServPort,
			pInfo->csServUser,pInfo->csServPassword))
		{
			//Log(L_WARNING,L"��¼ʧ��!");
			continue;
		}

		if(!pDownLoadCmd->DownloadFile(pInfo->csLocFileDir,pInfo->csTPFileName,&pInfo->ullTPFileByte,pInfo->csServFileDir,&(pInfo->ullTPDestnFileSize),
			&(pInfo->ullTpFileSize),pInfo->hEvtEndModule,pInfo->bIsPort,pInfo->bIsForce))
		{
			continue;
		}
	
		bRet = TRUE;
		//pInfo->pCarInfo->Lock();
		pInfo->pCarInfo->UpdateLocFileInfo(pInfo->strCarId,pInfo->strTm,pInfo->strLocFilePath,pInfo->nLocFileType);
		//pInfo->pCarInfo->Unlock();

		TRACE( L"%s�ļ� ��%d�ν���%s\n", pInfo->csTPFileName, i,L"����");
		break;

	}

	delete pDownLoadCmd;
	delete pInfo;
	return 0;
}

//
UINT ThreadProc_LoadCarId(LPVOID lParam)
{
	CFileView*    pFileView = (CFileView*)lParam;
	CCarFileInfo* pFileInfo = pFileView->m_pCarFileInfo;
	CDbOperation dbOperator;

	//	���CDbOperation���߳��У�������ڴ��߳���CoInitialize(NULL)
	CoInitialize(NULL);

	dbOperator.GetCarName(theApp.m_strDbFilePath);
	//
	pFileView->m_ComboxCarSelect.EnableWindow(FALSE);

	CString result;
	pFileInfo->Lock();
	while (!dbOperator.RecordSet.IsEOF())
	{
		result = dbOperator.RecordSet.GetValueString(L"CAR_NAME");

		pFileInfo->AddCarId(result);
		dbOperator.RecordSet.MoveNext();
	}
	pFileInfo->Unlock();

	pFileView->InitComboxCarSelCtrl();

	pFileView->m_ComboxCarSelect.EnableWindow(TRUE);
	return 0;

}
// CFileView

IMPLEMENT_DYNCREATE(CFileView, CFormView)

CFileView::CFileView()
	: CFormView(CFileView::IDD)
{
	m_pLoadDataThread = NULL;
	m_pLoadLocFileThread = NULL;
	m_pCarFileInfo = new CCarFileInfo;
}

CFileView::~CFileView()
{
	if(m_pCarFileInfo)
		delete m_pCarFileInfo;
	if(m_pLoadDataThread)
		::WaitForSingleObject(m_pLoadDataThread->m_hThread,0);
	if(m_pLoadLocFileThread)
		::WaitForSingleObject(m_pLoadLocFileThread->m_hThread,0);

}

void CFileView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_TimeStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_DateEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_TimeEnd);
	DDX_Control(pDX, IDC_COMBO2, m_ComboxCarSelect);
	DDX_Control(pDX, IDC_COMBO3, m_ComboxPlaySource);
}

BEGIN_MESSAGE_MAP(CFileView, CFormView)
		ON_WM_DESTROY()
		ON_WM_MOUSEACTIVATE()
		ON_BN_CLICKED(IDC_BUTTON2, &CFileView::OnBnClickedButton2)
END_MESSAGE_MAP()


// CFileView ���

#ifdef _DEBUG
void CFileView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFileView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

BOOL CFileView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFileView::OnDestroy()
{
	CWnd::OnDestroy();  //����ΪCWnd����  
}

void CFileView::PostNcDestroy()
{

}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);  
}

int CFileView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CFileView::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{

}

#endif
#endif //_DEBUG


// CFileView ��Ϣ�������

void CFileView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���

	//m_DateStart.SetFormat(L"yyyy-MM-dd   HH:mm:ss");
	//m_ComboxPlaySource.SetCurSel(0);
	BeginInitComboxCarCtrl();
	BeginLoadLocFileThread();

}
#include "MainFrm.h"
void CFileView::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	((CMainFrame*)theApp.m_pMainWnd)->GetChildView();

	if( m_ComboxCarSelect.GetCurSel() < 0)
	{
		MessageBox(L"δѡ�л���",L"�����ļ�");
		return;
	}
	if(m_ComboxPlaySource.GetCurSel() < 0)
	{
		MessageBox(L"δѡ�в���Դ",L"�����ļ�");
		return;
	}
	if(!TimeCheck())
	{
		return ;
	}
	//	��ֹ�޸�
	ChangeDateTimeUI(FALSE);

	if(m_ComboxPlaySource.GetCurSel() == 1)
		BeginLoadDataThread();
	else
	{

		PlayBack();
		ChangeDateTimeUI(TRUE);
	}

	return;
}

BOOL CFileView::TimeCheck()
{
	CTime tmStart,tmEnd;

	if( !GetDateTime(tmStart,0) )
	{
		return FALSE;
	}

	if( !GetDateTime(tmEnd,1) )
	{
		return FALSE;
	}

	if(tmStart > tmEnd || (tmStart < INIT_STOREINFO_DATETIME) || tmEnd > CTime::GetCurrentTime())
	{
		MessageBox(L"ʱ�䲻���Ϲ淶");
		return FALSE;
	}

	return TRUE;
}

void CFileView::BeginLoadDataThread()
{	
	//	������ڼ�������
	if(m_pLoadDataThread)		// ���߳��Ѿ����У���ȴ�����
	{
		if(::WaitForSingleObject(m_pLoadDataThread->m_hThread,0) != WAIT_OBJECT_0)
		{	
			MessageBox(L"����һ����������ִ��,�Ժ�����",L"�����ļ�");
			return ;
		}
		delete m_pLoadDataThread;
		m_pLoadDataThread=NULL;
	}

	if(m_pLoadDataThread = AfxBeginThread(ThreadProc_LoadFileViewData,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pLoadDataThread->m_bAutoDelete=FALSE;
		m_pLoadDataThread->ResumeThread();
	}
	return;

}

//	��ȡ�ؼ���ǰʱ�� Ĭ�ϲ��� 0 ��ʾ��ʼʱ��  
BOOL CFileView::GetDateTime(CTime &tmR,int nDateTimeType/* = 0*/)
{
	CTime tmDateS,tmTimeS,tmDateE,tmTimeE;
	if(nDateTimeType == 0)
	{
		if(m_DateStart.GetTime(tmDateS) != GDT_VALID || m_TimeStart.GetTime(tmTimeS) != GDT_VALID )
		{
			MessageBox(L"��ʼʱ��δ����",L"�����ļ�");
			return FALSE;
		}
		CTime tmStart(tmDateS.GetYear(),tmDateS.GetMonth(),tmDateS.GetDay(),tmTimeS.GetHour(),tmTimeS.GetMinute(),tmTimeS.GetSecond());
		tmR = tmStart;
		return TRUE;
	}
	
	if(m_DateEnd.GetTime(tmDateE) != GDT_VALID || m_TimeEnd.GetTime(tmTimeE) != GDT_VALID )
	{
		MessageBox(L"����ʱ��δ����",L"�����ļ�");
		return FALSE;
	}
	CTime tmEnd(tmDateE.GetYear(),tmDateE.GetMonth(),tmDateE.GetDay(),tmTimeE.GetHour(),tmTimeE.GetMinute(),tmTimeE.GetSecond());
	tmR = tmEnd;
	return TRUE;
}

void CFileView::PlayBack()
{	
	CTime tmS,tmE;
	GetDateTime(tmS,0);
	GetDateTime(tmE,1);
	
	CString strCarId;
	BOOL	bIsVideo = TRUE;
	m_ComboxCarSelect.GetWindowText(strCarId);
	
	CChildView* pView = (CChildView*)GetChildView();

	m_pCarFileInfo->Lock();

	for(std::map<CString,CFilePath*>::iterator it = m_pCarFileInfo->GetFileIter(strCarId,tmS,1) ; it!=m_pCarFileInfo->GetFileIter(strCarId,tmE,0) ;it++)
	{
		pView->SendMessage(WM_ADDFILELIST,(WPARAM)&(it->second->strServVideoPath),(LPARAM)&bIsVideo);
	}

	m_pCarFileInfo->Unlock();

}

void CFileView::ChangeDateTimeUI(BOOL bEnable)
{
	m_DateStart.EnableWindow(bEnable);
	m_DateEnd.EnableWindow(bEnable);
	m_TimeStart.EnableWindow(bEnable);
	m_TimeEnd.EnableWindow(bEnable);

	GetDlgItem(IDC_BUTTON2)->EnableWindow(bEnable);
	m_ComboxCarSelect.EnableWindow(bEnable);
	m_ComboxPlaySource.EnableWindow(bEnable);
}

CView* CFileView::GetChildView()
{
	POSITION rPos =  theApp.m_pPlay->GetFirstDocPosition();
	if(rPos == 0)
	{
		theApp.m_pPlay->OpenDocumentFile(NULL);
	}

	rPos =  theApp.m_pPlay->GetFirstDocPosition();
	CDocument* pDoc = theApp.m_pPlay->GetNextDoc(rPos);

	CView *pView;
	if(pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();   //��ѯ��doc�µ������Ѵ���view
		while (pos != NULL)
		{
			pView = pDoc->GetNextView(pos);
			if (pView->IsKindOf(RUNTIME_CLASS(CChildView)))   //����Ѵ�����view ������Ϊ�
			{ 
				// the requested view class has already been created; show it
				//CFrameWnd* pWnd= pView->GetParentFrame();
				//pWnd->ActivateFrame();  
				pView->UpdateWindow();//��ͼ������ʾ��
				return pView;
			}
		}
	}
}

void CFileView::BeginInitComboxCarCtrl()
{
	AfxBeginThread(ThreadProc_LoadCarId,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,0,0,NULL);

}

void CFileView::InitComboxCarSelCtrl()
{
	m_ComboxCarSelect.ResetContent();

	m_pCarFileInfo->Lock();
	for(std::map<CString,CFileInfo*>::iterator it = m_pCarFileInfo->m_mpFileInfo.begin() ; it!=m_pCarFileInfo->m_mpFileInfo.end() ; it++)
	{
		m_ComboxCarSelect.AddString(it->first);
	}
	m_pCarFileInfo->Unlock();
	
}

void CFileView::BeginLoadLocFileThread()
{
	//	������ڼ�������
	if(m_pLoadLocFileThread)		// ���߳��Ѿ����У���ȴ�����
	{
		if(::WaitForSingleObject(m_pLoadLocFileThread->m_hThread,0) != WAIT_OBJECT_0)
		{	
			MessageBox(L"����һ����������ִ��,�Ժ�����",L"�����ļ�");
			return ;
		}
		delete m_pLoadLocFileThread;
		m_pLoadLocFileThread=NULL;
	}

	if(m_pLoadLocFileThread = AfxBeginThread(ThreadProc_LoadlocFile,(LPVOID)this,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pLoadLocFileThread->m_bAutoDelete=FALSE;
		m_pLoadLocFileThread->ResumeThread();
	}
	return;
}


BOOL CFileView::FindLocFile(CString strFilePath,BOOL bIsVideoFile /*= TRUE*/)
{
	CString  strFileName,strCarId,strDatetime;
	CFileFind fileFind;
	BOOL bIsFinded;

	bIsFinded = fileFind.FindFile( strFilePath );
	if( !bIsFinded )
		return FALSE;

	m_pCarFileInfo->Lock();
	while(bIsFinded)
	{
		bIsFinded=fileFind.FindNextFile();
		//�����ļ�����ʾ
		if(fileFind.IsHidden())continue;
		//Ŀ¼�Թ�
		if(fileFind.IsDirectory())continue;
		strFileName = fileFind.GetFileName();
		if(GetCarAndDateFromFileName(strFileName,strCarId,strDatetime,bIsVideoFile))
		{
			m_pCarFileInfo->UpdateLocFileInfo(strCarId,strDatetime,fileFind.GetFilePath(),bIsVideoFile);
		}
	}

	m_pCarFileInfo->Unlock();
	return TRUE;
}
