// AnalysisView.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayDemo.h"
#include "AnalysisView.h"
#include "MainFrm.h"
#include "DlgCarSelect.h"


UINT ThreadProc_LoadAnalyViewData(LPVOID lParam)
{
	CAnalysisView *pAnalyView = (CAnalysisView*)lParam;
	pAnalyView->m_pAnalyInfo->Lock();

	//	设置时间限制
	pAnalyView->m_DateTimeStart.GetTime(pAnalyView->m_pAnalyInfo->tmStart);
	pAnalyView->m_DateTimeEnd.GetTime(pAnalyView->m_pAnalyInfo->tmEnd);

	if( !pAnalyView->m_pAnalyInfo->LoadData())
	{
		AfxMessageBox(L"数据库连接失败");
		pAnalyView->m_pAnalyInfo->Unlock();
		pAnalyView->m_ComboxCarSelect.EnableWindow(TRUE);
		return 0;
	}
	pAnalyView->m_pAnalyInfo->Unlock();

	//	更新控件信息
	pAnalyView->UpdateComboxCtrl();
	pAnalyView->OnUpdateData();
	return 0;
}

// CAnalysisView

IMPLEMENT_DYNCREATE(CAnalysisView, CFormView)

CAnalysisView::CAnalysisView()
	: CFormView(CAnalysisView::IDD)
{
	m_nFillGradientType = 0;
	m_bShowDataLabels = TRUE;
	m_nDataLabelPosition = 2;
	m_bDisplayShadow = TRUE;
	m_nColorType = 0;
	m_bInitUpdate = FALSE;
	m_mpCarSelect.clear();
	m_nDateBtnSelect = 0;
	m_pAnalyInfo   =  new CAnalysisInfo;
	m_pLoadDataThread = NULL;
}

CAnalysisView::~CAnalysisView()
{
	if(m_pAnalyInfo)
		delete m_pAnalyInfo;
}

void CAnalysisView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_YEAR, m_btnYear);
	DDX_Control(pDX, IDC_BTN_MONTH, m_btnMonth);
	DDX_Control(pDX, IDC_BTN_DAY, m_btnDay);
	DDX_Control(pDX, IDC_BTN_HOUR, m_btnHour);
	DDX_Control(pDX, IDC_DATETIME1_START, m_DateTimeStart);
	DDX_Control(pDX, IDC_DATETIME1_END, m_DateTimeEnd);
	DDX_Control(pDX, IDC_DATETIME1_ASK, m_DateTimeAsk);
	DDX_Control(pDX, IDC_COMBO1_CARSELECT, m_ComboxCarSelect);
	//DDX_Control(pDX, IDC_SERIES_SHADOW, m_wndDisplayShadow);
	DDX_Control(pDX, IDC_DATA_LABEL_POSITION, m_wndDataLabelPosition);
	DDX_Control(pDX, IDC_CHART_YEAR, m_wndChart);
//	DDX_Control(pDX, IDC_CHART_MONTH, m_wndChartMonth);
//	DDX_Control(pDX, IDC_CHART_DAY, m_wndChartDay);
//	DDX_Control(pDX, IDC_CHART_HOUR, m_wndChartHour);
	DDX_CBIndex(pDX, IDC_FILL_GRADIENT_TYPE, m_nFillGradientType);
	DDX_Check(pDX, IDC_DATA_LABELS, m_bShowDataLabels);
	DDX_CBIndex(pDX, IDC_DATA_LABEL_POSITION, m_nDataLabelPosition);
	DDX_CBIndex(pDX, IDC_COLOR_TYPE, m_nColorType);

}

BEGIN_MESSAGE_MAP(CAnalysisView, CFormView)

	ON_BN_CLICKED(IDC_DATA_LABELS, OnUpdateChart)
	ON_CBN_SELENDOK(IDC_DATA_LABEL_POSITION, OnUpdateChart)
	ON_CBN_SELENDOK(IDC_FILL_GRADIENT_TYPE, OnUpdateChart)
	ON_CBN_SELENDOK(IDC_COLOR_TYPE, OnUpdateChart)
	//ON_BN_CLICKED(IDC_SERIES_SHADOW, OnUpdateChart)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
	ON_CBN_SELCHANGE(IDC_COMBO1_CARSELECT, &CAnalysisView::OnCbnSelchangeCombo1Carselect)
	ON_COMMAND_RANGE(IDC_BTN_YEAR, IDC_BTN_HOUR, OnUpdateDateSelect)
	ON_BN_CLICKED(IDC_BTN_ASK, &CAnalysisView::OnBnClickedBtnAsk)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIME1_ASK, &CAnalysisView::OnDtnDatetimechangeDatetime1Ask)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CAnalysisView)

	EASYSIZE(IDC_CHART_YEAR,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_GROUP_DATASHOW,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_YEAR,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW,0)
	EASYSIZE(IDC_BTN_MONTH,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW,0)
	EASYSIZE(IDC_BTN_DAY,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW,0)
	EASYSIZE(IDC_BTN_HOUR,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW,0)
	EASYSIZE(IDC_DATETIME1_ASK,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW,0)
	EASYSIZE(IDC_GROUP_ASK,ES_BORDER,IDC_CHART_YEAR,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_CAR,ES_BORDER,IDC_GROUP_ASK,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_COMBO1_CARSELECT,ES_BORDER,IDC_GROUP_ASK,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_TIME,ES_BORDER,IDC_GROUP_ASK,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_DATETIME1_START,ES_BORDER,IDC_GROUP_ASK,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_TIMEEND,ES_BORDER,IDC_GROUP_ASK,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_DATETIME1_END,ES_BORDER,IDC_GROUP_ASK,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BTN_ASK,ES_BORDER,IDC_GROUP_ASK,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_GROUP_CHART,ES_KEEPSIZE,IDC_CHART_YEAR,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_DATA_LABELS,ES_KEEPSIZE,IDC_GROUP_CHART,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_COLOR,ES_KEEPSIZE,IDC_GROUP_CHART,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_COLOR_TYPE,ES_KEEPSIZE,IDC_GROUP_CHART,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_FONT,ES_KEEPSIZE,IDC_GROUP_CHART,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_DATA_LABEL_POSITION,ES_KEEPSIZE,IDC_GROUP_CHART,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_STYLE,ES_KEEPSIZE,IDC_GROUP_CHART,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_FILL_GRADIENT_TYPE,ES_KEEPSIZE,IDC_GROUP_CHART,ES_BORDER,ES_BORDER,0)


END_EASYSIZE_MAP

// CAnalysisView 诊断

#ifdef _DEBUG
void CAnalysisView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAnalysisView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAnalysisView 消息处理程序

void CAnalysisView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

	ResizeParentToFit();

	//
	CComboBox	*comboButton = ((CComboBox*)GetDlgItem(IDC_COLOR_TYPE));
	comboButton->AddString (_T("Default Theme"));
	comboButton->AddString (_T("Pastel Theme"));
	comboButton->AddString (_T("Spring Theme"));
	comboButton->AddString (_T("Forest Green Theme"));
	comboButton->AddString (_T("Sea Blue Theme"));
	comboButton->AddString (_T("Golden Theme"));
	comboButton->AddString (_T("Dark Rose Theme"));
	comboButton->AddString (_T("Black and Gold Theme"));
	comboButton->AddString (_T("Rainbow Theme"));
	comboButton->AddString (_T("Gray Theme"));
	comboButton->AddString (_T("Arctic Theme"));
	comboButton->AddString (_T("Black and Red Theme"));
	comboButton->AddString (_T("Plum Theme"));
	comboButton->AddString (_T("Sunny Theme"));
	comboButton->AddString (_T("Violet Theme"));
	comboButton->AddString (_T("Flower Theme"));
	comboButton->AddString (_T("Steel Theme"));
	comboButton->AddString (_T("Gray and Green Theme"));
	comboButton->AddString (_T("Olive Theme"));
	comboButton->AddString (_T("Autumn Theme"));
	comboButton->AddString (_T("Black and Green Theme"));
	comboButton->AddString (_T("Black and Blue Theme"));
	comboButton->AddString (_T("Flat 2014 1-st Theme"));
	comboButton->AddString (_T("Flat 2014 2-nd Theme"));
	comboButton->AddString (_T("Flat 2014 3-rd Theme"));
	comboButton->AddString (_T("Flat 2014 4-th Theme"));
	comboButton->SetCurSel(0);
	
	//
	InitBtn();

	//
	CTime timeS,timeA;
	CString csTime;
	timeA = CTime::GetCurrentTime();
	timeS = timeA - CTimeSpan(1,0,0,0);

	m_DateTimeStart.SetTime(&timeS);
	m_DateTimeEnd.SetTime(&timeA);
	m_DateTimeAsk.SetTime(&timeA);
	
	ChangeDateGroupCtlUI(FALSE);

	//
	InitChart();

	// 莫名BUG 注意此语句调用在chart控件生成后 
	GetParent()->SetWindowText(L"数据分析(柱状统计图)");

	//OnUpdateData();

	//	调整控件位置  勿动
	m_bInitUpdate = TRUE;
	((CMainFrame*)AfxGetMainWnd())->AdaptWindowSize();
}

void CAnalysisView::InitChart()
{
	BCGPChartCategory category = BCGPChartColumn; //
	BCGPChartType type = BCGP_CT_SIMPLE;

	CBCGPChartVisualObject* pChartY = m_wndChart.GetChart();
	ASSERT_VALID(pChartY);


	pChartY->SetChartType(category, type);

	pChartY->SetChartTitle(_T("机车数据"));
}

void CAnalysisView::OnUpdateChart()
{
	UpdateData();

	m_wndDataLabelPosition.EnableWindow(m_bShowDataLabels);

	//	
	UpdateChart();
}

void CAnalysisView::UpdateChart()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->ShowDataLabels(m_bShowDataLabels);

	if (m_bShowDataLabels)
	{
		BCGPChartDataLabelOptions dataLabelOptions = pChart->GetDataLabelOptions();
		dataLabelOptions.m_position = (BCGPChartDataLabelOptions::LabelPosition)m_nDataLabelPosition;

		pChart->SetDataLabelsOptions(dataLabelOptions);
	}

	CBCGPBrush::BCGP_GRADIENT_TYPE type;

	switch (m_nFillGradientType)
	{
	case 0:
	default:
		type = (CBCGPBrush::BCGP_GRADIENT_TYPE)-1;
		break;

	case 1:
		type = CBCGPBrush::BCGP_NO_GRADIENT;
		break;

	case 2:
		type = CBCGPBrush::BCGP_GRADIENT_HORIZONTAL;
		break;

	case 3:
		type = CBCGPBrush::BCGP_GRADIENT_VERTICAL;
		break;

	case 4:
		type = CBCGPBrush::BCGP_GRADIENT_BEVEL;
		break;

	case 5:
		type = pChart->GetChartCategory() == BCGPChartBar ? 
			CBCGPBrush::BCGP_GRADIENT_PIPE_HORIZONTAL : CBCGPBrush::BCGP_GRADIENT_PIPE_VERTICAL;
		break;
	}

	for (int i = 0; i < pChart->GetSeriesCount(); i++)
	{
		CBCGPChartSeries* pSeries = pChart->GetSeries(i);
		ASSERT_VALID(pSeries);

		pSeries->SetDefaultFillGradientType(type);
	}

	pChart->SetSeriesShadow(m_bDisplayShadow);

	pChart->SetColors((CBCGPChartTheme::ChartTheme)m_nColorType);

	pChart->Redraw();
}

BOOL CAnalysisView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rc;
	//GetClientRect(&rc);
	//pDC->FillSolidRect(&rc ,  RGB(225,225,255)  ); 
	//return true;
	return CFormView::OnEraseBkgnd(pDC);
}

void CAnalysisView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFormView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CAnalysisView::OnSize(UINT nType, int cx, int cy)
{
	static BOOL s_bInit = FALSE;
	if( theApp.m_bInit && !s_bInit && m_bInitUpdate)
	{
		s_bInit = TRUE;
		INIT_EASYSIZE;
	}

	if(s_bInit)	
		UPDATE_EASYSIZE;

	//CFormView::OnSize(nType, cx, cy);

	if (::IsWindow(m_wndChart.GetSafeHwnd()) && m_wndChart.GetChart())
	{
		OnUpdateChart();
	}
}

void CAnalysisView::OnUpdateData()
{
	//	如果正在加载数据
	if(m_pLoadDataThread)		// 若线程已经运行，则等待结束
	{
		if(::WaitForSingleObject(m_pLoadDataThread->m_hThread,0) != WAIT_OBJECT_0)
		{	
			MessageBox(L"正在加载数据");
			return ;
		}
	}

	UpdateData();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	CBCGPChartBarSeries* pBarSeries = NULL;
	
	CTime tmS,tmE,tmA;
	CString tmName;
	m_DateTimeStart.GetTime(tmS);
	m_DateTimeEnd.GetTime(tmE);
	m_DateTimeAsk.GetTime(tmA);

	m_pAnalyInfo->Lock();
	pChart->CleanUpChartData();
	for(std::map<CString,BOOL>::iterator it=m_mpCarSelect.begin(); it!=m_mpCarSelect.end() ; it++)
	{
		if(!it->second)
			continue;
		
		pBarSeries = DYNAMIC_DOWNCAST(CBCGPChartBarSeries, pChart->CreateSeries(it->first));
		
		//	展示 年 数据视图
		if(m_nDateBtnSelect == DateSelectType::YEAR)
		{
			for(int j=tmS.GetYear() ; j <= tmE.GetYear() ; j++)
			{
				tmName.Format(L"%d年",j);
				pBarSeries->AddDataPoint(tmName, m_pAnalyInfo->GetData(it->first,j));
			}
		}
		
		//	展示 月 数据视图
		if(m_nDateBtnSelect == DateSelectType::MONTH)
		{
			for(int j=1 ; j <= 12 ; j++)
			{
				tmName.Format(L"%d月",j);
				pBarSeries->AddDataPoint(tmName, m_pAnalyInfo->GetData(it->first,tmA.GetYear(),j));
			}
		}

		//	展示 天 数据视图
		if(m_nDateBtnSelect == DateSelectType::DAY)
		{
			for(int j=1 ; j <= GetDayNumByYearMonth(tmA.GetYear(),tmA.GetMonth()) ; j++)
			{
				tmName.Format(L"%d日",j);
				pBarSeries->AddDataPoint(tmName, m_pAnalyInfo->GetData(it->first,tmA.GetYear(),tmA.GetMonth(),j));
			}
		}

		//	展示 小时 数据视图
		if(m_nDateBtnSelect == DateSelectType::HOUR)
		{
			for(int j=0 ; j <= 23 ; j++)
			{
				tmName.Format(L"%d时",j);
				pBarSeries->AddDataPoint(tmName, m_pAnalyInfo->GetData(it->first,tmA.GetYear(),tmA.GetMonth(),tmA.GetDay(),j));
			}
		}
	}

	m_pAnalyInfo->Unlock();
	OnUpdateChart();
}
void CAnalysisView::OnCbnSelchangeCombo1Carselect()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_ComboxCarSelect.GetCurSel();
	//	自定义
	if(nSel == m_ComboxCarSelect.GetCount()-1)
	{
		CDlgCarSelect dlg;
		dlg.InitData(&m_mpCarSelect);
		if(dlg.DoModal() != IDOK)
		{
			return;
		}
	}
	else
	{
		CString strCarSel;
		m_ComboxCarSelect.GetLBText(nSel,strCarSel);
		for(std::map<CString,BOOL>::iterator it=m_mpCarSelect.begin(); it!=m_mpCarSelect.end() ; it++)
		{
			it->second = FALSE;
		}
		m_mpCarSelect[strCarSel] = TRUE;
	}
		
	OnUpdateData();
	return;
}

void CAnalysisView::OnBnClickedBtnAsk()
{
	// TODO: 在此添加控件通知处理程序代码

	static BOOL bIsAsk = TRUE;

	if(!bIsAsk)
	{
		bIsAsk = TRUE;
		ChangeDateGroupCtlUI(FALSE);
		GetDlgItem(IDC_BTN_ASK)->SetWindowText(L"开始查询");
		return;
	}
	CTime tmEnd,tmAsk;
	m_DateTimeEnd.GetTime(tmEnd);
	m_DateTimeAsk.SetTime(&tmEnd);

	if(!TimeCheck())
	{
		return;
	}
	//	数据库加载数据
	LoadData();

	bIsAsk = FALSE;

	GetDlgItem(IDC_BTN_ASK)->SetWindowText(L"重新设置");

	ChangeDateGroupCtlUI(TRUE);

	m_nDateBtnSelect = DateSelectType::YEAR;

}
BOOL CAnalysisView::TimeCheck()
{
	CTime tmStart,tmEnd,tmAsk;
	if(m_DateTimeStart.GetTime(tmStart) != GDT_VALID)
	{
		MessageBox(L"初始时间未设置");
		return FALSE;
	}

	if(m_DateTimeEnd.GetTime(tmEnd) != GDT_VALID)
	{
		MessageBox(L"结束时间未设置");
		return FALSE;
	}

	if(m_DateTimeAsk.GetTime(tmAsk) != GDT_VALID)
	{
		//MessageBox(L"结束时间未设置");
		return FALSE;
	}

	if(tmStart > tmEnd || (tmStart < INIT_STOREINFO_DATETIME) || tmEnd > CTime::GetCurrentTime())
	{
		MessageBox(L"时间不符合规范");
		return FALSE;
	}

	if(tmAsk < tmStart || tmAsk > tmEnd)
	{
		MessageBox(L"时间应在查询设置初始-结束期间内");
		return FALSE;
	}
	
	return TRUE;
}

void CAnalysisView::OnUpdateDateSelect(UINT uID)
{
	m_nDateBtnSelect = DateSelectType(uID - IDC_BTN_YEAR);
	
	CButtonST* pBtn = NULL;
	for(UINT uId = IDC_BTN_YEAR ; uId <= IDC_BTN_HOUR ; uId++)
	{
		pBtn = (CButtonST*)GetDlgItem(uId);
		
		if(uId == uID)
			pBtn->SetBitmaps(IDB_BTN_BKG1, RGB(255, 255, 255));
		else
			pBtn->SetBitmaps(IDB_BTN_BKG, RGB(255, 255, 255));
		pBtn->SizeToContent();

	}

	OnUpdateData();

}

void CAnalysisView::ChangeDateGroupCtlUI(BOOL bEnable)
{
	for(UINT i=IDC_BTN_YEAR; i<=IDC_BTN_HOUR ; i++)
		GetDlgItem(i)->EnableWindow(bEnable);

	GetDlgItem(IDC_DATETIME1_ASK)->EnableWindow(bEnable);
	m_ComboxCarSelect.EnableWindow(bEnable);

	m_DateTimeStart.EnableWindow(!bEnable);
	m_DateTimeEnd.EnableWindow(!bEnable);
}
void CAnalysisView::OnDtnDatetimechangeDatetime1Ask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	if(!TimeCheck())
	{
		return;
	}
	
	OnUpdateData();
}

void CAnalysisView::LoadData()
{

	BeginLoadDataThread((LPVOID)this);

	//	线程任务结束解锁
	//m_pAnalyInfo->Unlock();
}


void CAnalysisView::UpdateComboxCtrl()
{
	m_ComboxCarSelect.ResetContent();

	m_pAnalyInfo->Lock();
	for(std::map<CString,CCarInfo*>::iterator it=m_pAnalyInfo->mpCarInfo.begin(); it!=m_pAnalyInfo->mpCarInfo.end(); it++)
	{
		m_ComboxCarSelect.AddString(it->first);
		m_mpCarSelect[it->first] = 0;
	}
	m_pAnalyInfo->Unlock();
	m_ComboxCarSelect.AddString(L"自定义");
	//m_ComboxCarSelect.SetCurSel(0);
}


void CAnalysisView::BeginLoadDataThread(LPVOID lpVoid)
{
	if(m_pLoadDataThread)		// 若线程已经运行，则等待结束
	{
		::WaitForSingleObject(m_pLoadDataThread->m_hThread,0);
		delete m_pLoadDataThread;
		m_pLoadDataThread=NULL;
	}

	if(m_pLoadDataThread = AfxBeginThread(ThreadProc_LoadAnalyViewData,lpVoid,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_ComboxCarSelect.EnableWindow(FALSE);

		m_pLoadDataThread->m_bAutoDelete=FALSE;
		m_pLoadDataThread->ResumeThread();
	}
	return;
}


void CAnalysisView::InitBtn()
{
	static BOOL bIsInit = FALSE;

	CButtonST* pBtn = NULL;
	for(UINT uId = IDC_BTN_YEAR ; uId <= IDC_BTN_HOUR ; uId++)
	{
		pBtn = (CButtonST*)GetDlgItem(uId);
		if(!bIsInit)
		{
			pBtn->SetBitmaps(IDB_BTN_BKG, RGB(255, 255, 255));
			pBtn->SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
			pBtn->SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
			pBtn->SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 196, 10));
			pBtn->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
			pBtn->SizeToContent();
			pBtn->DrawBorder(FALSE, FALSE);
		}
		
	}

	bIsInit = TRUE;
}