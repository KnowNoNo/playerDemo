// ElementsView.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayDemo.h"
#include "ElementsView.h"
#include "MainFrm.h"
#include "DlgCarSelect.h"

//
UINT ThreadProc_LoadElementViewData(LPVOID lParam)
{
	CElementsView *pAnalyView = (CElementsView*)lParam;
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


// CElementsView

IMPLEMENT_DYNCREATE(CElementsView, CFormView)

CElementsView::CElementsView()
	: CFormView(CElementsView::IDD)
{
	m_bXAxis = TRUE;
	m_bXAxisName = FALSE;
	m_bYAxis = TRUE;
	m_bYAxisName = FALSE;
	m_bDataLabels = TRUE;
	m_bDataMarkers = TRUE;
	m_bLegend = TRUE;
	m_bLegendOverlapsChart = FALSE;
	m_bTitle = TRUE;
	m_bTitleOverlapsChart = FALSE;
	m_bXIntervalInterlasing = TRUE;
	m_bYIntervalInterlasing = FALSE;
	m_bYAxisGridLines = TRUE;
	m_bXAxisGridLines = TRUE;
	m_bColoredZones = FALSE;
	m_bCustomLines = FALSE;
	m_bTextObjects = FALSE;
	m_bDataTable = TRUE;
	m_bDataTableLegendKeys = TRUE;
	m_bXAxisReversed = FALSE;

	m_strokeStyle.SetDashStyle(CBCGPStrokeStyle::BCGP_DASH_STYLE_DASH_DOT);

	m_bInitUpdate = FALSE;

	m_mpCarSelect.clear();
	m_nDateBtnSelect = 0;
	m_pAnalyInfo   =  new CAnalysisInfo;
	m_pLoadDataThread = NULL;
}

CElementsView::~CElementsView()
{
	if(m_pAnalyInfo)
		delete m_pAnalyInfo;
}

void CElementsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_YEAR2, m_btnYear);
	DDX_Control(pDX, IDC_BTN_MONTH2, m_btnMonth);
	DDX_Control(pDX, IDC_BTN_DAY2, m_btnDay);
	DDX_Control(pDX, IDC_BTN_HOUR2, m_btnHour);
	DDX_Control(pDX, IDC_CHART2, m_wndChart);
	//DDX_Check(pDX, IDC_CHART_CUSTOM_LINES, m_bCustomLines);
	DDX_Check(pDX, IDC_DATA_TABLE, m_bDataTable);
	DDX_Check(pDX, IDC_DATA_LABELS2, m_bDataLabels);
	DDX_Check(pDX, IDC_CHART_AXIS_X_INTERVAL_INTERLASING, m_bXIntervalInterlasing);
	DDX_Check(pDX, IDC_CHART_AXIS_Y_INTERVAL_INTERLASING, m_bYIntervalInterlasing);
	DDX_Check(pDX, IDC_CHART_AXIS_Y_GRID_LINES, m_bYAxisGridLines);
	DDX_Check(pDX, IDC_CHART_AXIS_X_GRID_LINES, m_bXAxisGridLines);
	DDX_Control(pDX, IDC_DATETIME_START2, m_DateTimeStart);
	DDX_Control(pDX, IDC_DATETIME_END2, m_DateTimeEnd);
	DDX_Control(pDX, IDC_DATETIME_ASK2, m_DateTimeAsk);
	DDX_Control(pDX, IDC_COMBO_CARSELECT2, m_ComboxCarSelect);
}

BEGIN_MESSAGE_MAP(CElementsView, CFormView)
	//ON_BN_CLICKED(IDC_CHART_CUSTOM_LINES, OnUpdateChart)
	ON_BN_CLICKED(IDC_DATA_TABLE, OnUpdateChart)
	ON_BN_CLICKED(IDC_DATA_LABELS2, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_AXIS_X_INTERVAL_INTERLASING, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_AXIS_Y_INTERVAL_INTERLASING, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_AXIS_Y_GRID_LINES, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_AXIS_X_GRID_LINES, OnUpdateChart)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_CARSELECT2, OnCbnSelchangeComboCarselect2)
	ON_COMMAND_RANGE(IDC_BTN_YEAR2, IDC_BTN_HOUR2, OnUpdateDateSelect)
	ON_BN_CLICKED(IDC_BTN_ASK2, OnBnClickedBtnAsk2)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIME_ASK2, OnDtnDatetimechangeDatetimeAsk2)
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CElementsView)

	EASYSIZE(IDC_CHART2,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_GROUP_DATASHOW2,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BTN_YEAR2,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW2,0)
	EASYSIZE(IDC_BTN_MONTH2,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW2,0)
	EASYSIZE(IDC_BTN_DAY2,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW2,0)
	EASYSIZE(IDC_BTN_HOUR2,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW2,0)
	EASYSIZE(IDC_DATETIME_ASK2,ES_KEEPSIZE,ES_BORDER,ES_BORDER,IDC_GROUP_DATASHOW2,0)
	EASYSIZE(IDC_GROUP_ASK2,ES_BORDER,IDC_CHART2,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_CAR2,ES_BORDER,IDC_GROUP_ASK2,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_COMBO_CARSELECT2,ES_BORDER,IDC_GROUP_ASK2,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_TIME2,ES_BORDER,IDC_GROUP_ASK2,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_DATETIME_START2,ES_BORDER,IDC_GROUP_ASK2,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_TEXT_TIMEEND2,ES_BORDER,IDC_GROUP_ASK2,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_DATETIME_END2,ES_BORDER,IDC_GROUP_ASK2,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_BTN_ASK2,ES_BORDER,IDC_GROUP_ASK2,ES_KEEPSIZE,ES_BORDER,0)
	EASYSIZE(IDC_GROUP_CHART2,ES_KEEPSIZE,IDC_CHART2,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_DATA_LABELS2,ES_KEEPSIZE,IDC_GROUP_CHART2,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_DATA_TABLE,ES_KEEPSIZE,IDC_GROUP_CHART2,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_CHART_AXIS_X_GRID_LINES,ES_KEEPSIZE,IDC_GROUP_CHART2,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_CHART_AXIS_X_INTERVAL_INTERLASING,ES_KEEPSIZE,IDC_GROUP_CHART2,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_CHART_AXIS_Y_GRID_LINES,ES_KEEPSIZE,IDC_GROUP_CHART2,ES_BORDER,ES_BORDER,0)
	EASYSIZE(IDC_CHART_AXIS_Y_INTERVAL_INTERLASING,ES_KEEPSIZE,IDC_GROUP_CHART2,ES_BORDER,ES_BORDER,0)

END_EASYSIZE_MAP

// CElementsView 诊断

#ifdef _DEBUG
void CElementsView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CElementsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CElementsView 消息处理程序

void CElementsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类

		//((CMainFrame*)AfxGetMainWnd())->AdaptWindowSize();
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

	//
	InitBtn();

	m_brFill1 = CBCGPBrush(CBCGPColor::RoyalBlue, .2);
	m_brFill2 = CBCGPBrush(CBCGPColor::Peru, .2);
	m_brFill3 = CBCGPBrush(CBCGPColor::LightSkyBlue, CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT, .4);
	m_brFill4 = CBCGPBrush(CBCGPColor::LightYellow);
	m_brLine1 = CBCGPBrush(CBCGPColor::DarkRed);
	m_brLine2 = CBCGPBrush(CBCGPColor::DeepSkyBlue);
	m_brText1 = CBCGPBrush(CBCGPColor::SteelBlue);
	m_brText2 = CBCGPBrush(CBCGPColor::Brown);

	//	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	//CBCGPChartSeries* pSeries1 = pChart->CreateSeries(_T("Series 1"));
	//pSeries1->SetCurveType (BCGPChartFormatSeries::CCT_SPLINE);

	//CBCGPChartSeries* pSeries2 = pChart->CreateSeries(_T("Series 2"));
	//pSeries2->SetCurveType (BCGPChartFormatSeries::CCT_SPLINE_HERMITE);

	//pSeries1->AddDataPoint(0.);

	GetParent()->SetWindowText(L"数据分析(线性统计图)");
	
	m_bInitUpdate = TRUE;

}

void CElementsView::OnUpdateChart()
{
	UpdateData();

	//m_wndTitleOverlapsChart.EnableWindow(m_bTitle);
	//m_wndLegendOverlapsChart.EnableWindow(m_bLegend);
	//m_wndXAxisName.EnableWindow(m_bXAxis && !m_bDataTable);
	//m_wndYAxisName.EnableWindow(m_bYAxis);
	//m_wndXAxisInterlasing.EnableWindow(m_bXAxis);
	//m_wndYAxisInterlasing.EnableWindow(m_bYAxis);
	//m_wndXAxisGridLines.EnableWindow(m_bXAxis);
	//m_wndYAxisGridLines.EnableWindow(m_bYAxis);
	//m_wndDataTableLegendKeys.EnableWindow(m_bDataTable);

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->ShowChartTitle(m_bTitle, m_bTitleOverlapsChart);

	BCGPChartFormatDataTable formatDataTable;
	formatDataTable.m_bShowLegendKeys = m_bDataTableLegendKeys;
	formatDataTable.m_bInterlaceRows = TRUE;

	pChart->ShowDataTable(m_bDataTable, &formatDataTable);

	pChart->SetLegendPosition(m_bLegend ? BCGPChartLayout::LP_RIGHT : BCGPChartLayout::LP_NONE, m_bLegendOverlapsChart);

	pChart->ShowAxis(BCGP_CHART_X_PRIMARY_AXIS, m_bXAxis);
	pChart->ShowAxis(BCGP_CHART_Y_PRIMARY_AXIS, m_bYAxis);

	pChart->SetAxisName(BCGP_CHART_X_PRIMARY_AXIS, _T("X Axis"));
	pChart->ShowAxisName(BCGP_CHART_X_PRIMARY_AXIS, m_bXAxisName && !m_bDataTable);
	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS, m_bXIntervalInterlasing);
	pChart->ShowAxisGridLines(BCGP_CHART_X_PRIMARY_AXIS, m_bXAxisGridLines, FALSE);

	pChart->SetAxisName(BCGP_CHART_Y_PRIMARY_AXIS, _T("Y Axis"));
	pChart->ShowAxisName(BCGP_CHART_Y_PRIMARY_AXIS, m_bYAxisName);
	pChart->ShowAxisGridLines(BCGP_CHART_Y_PRIMARY_AXIS, m_bYAxisGridLines, FALSE);
	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS, m_bYIntervalInterlasing);

	pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS)->m_bReverseOrder = m_bXAxisReversed;

	pChart->ShowDataMarkers(m_bDataMarkers);
	pChart->ShowDataLabels(m_bDataLabels);

	//pChart->ShowSeries(m_bSecondSeries, 1);

	pChart->RemoveAllChartObjects();

	if (m_bColoredZones)
	{
		pChart->AddChartRangeObject(2, 6, TRUE /* Horizontal */, m_brFill1);
		pChart->AddChartRangeObject(6, 7.5, FALSE /* Vertical */, m_brFill2);
	}

	if (m_bCustomLines)
	{
		pChart->AddChartLineObject(5.0, TRUE /* Horizontal */, m_brLine1, 2, &m_strokeStyle);
		pChart->AddChartLineObject(5.0, FALSE /* Vertical */, m_brLine2, 2, &m_strokeStyle);
	}

	if (m_bTextObjects)
	{
		pChart->AddChartObject(new CBCGPChartTextObject(pChart, _T("BCGSoft Chart\nText Object 1"), 1, 8.5, m_brText1, m_brFill3, m_brText1));

		CBCGPChartTextObject* pTextObject2 = new CBCGPChartTextObject
			(pChart, _T("Text Object 2"), 4, 3, m_brText2, m_brFill4, m_brText2, 50, 135 /* Angle */, TRUE);
		pChart->AddChartObject(pTextObject2);
	}

	pChart->Redraw();
}

void CElementsView::OnSize(UINT nType, int cx, int cy)
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

void CElementsView::InitChart()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->SetSeriesShadow();

	pChart->SetChartTitle(_T("机车数据"));
}


BOOL CElementsView::TimeCheck()
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


void CElementsView::OnDtnDatetimechangeDatetimeAsk2(NMHDR *pNMHDR, LRESULT *pResult)
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

void CElementsView::OnBnClickedBtnAsk2()
{
	// TODO: 在此添加控件通知处理程序代码

	static BOOL bIsAsk = TRUE;

	if(!bIsAsk)
	{
		bIsAsk = TRUE;
		ChangeDateGroupCtlUI(FALSE);
		GetDlgItem(IDC_BTN_ASK2)->SetWindowText(L"开始查询");
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

	GetDlgItem(IDC_BTN_ASK2)->SetWindowText(L"重新设置");

	ChangeDateGroupCtlUI(TRUE);

	m_nDateBtnSelect = DateSelectType::YEAR;
}

void CElementsView::OnCbnSelchangeComboCarselect2()
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

void CElementsView::OnUpdateDateSelect(UINT uID)
{
	m_nDateBtnSelect = DateSelectType(uID - IDC_BTN_YEAR2);
	
	CButtonST* pBtn = NULL;
	for(UINT uId = IDC_BTN_YEAR2 ; uId <= IDC_BTN_HOUR2 ; uId++)
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

void CElementsView::OnUpdateData()
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
	CBCGPChartSeries* pBarSeries = NULL;


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

		pBarSeries =  pChart->CreateSeries(it->first);
		pBarSeries->SetCurveType (BCGPChartFormatSeries::CCT_SPLINE);

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

void CElementsView::UpdateComboxCtrl()
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
}

void CElementsView::BeginLoadDataThread(LPVOID lpVoid)
{
	if(m_pLoadDataThread)		// 若线程已经运行，则等待结束
	{
		::WaitForSingleObject(m_pLoadDataThread->m_hThread,0);
		delete m_pLoadDataThread;
		m_pLoadDataThread=NULL;
	}

	if(m_pLoadDataThread = AfxBeginThread(ThreadProc_LoadElementViewData,lpVoid,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_ComboxCarSelect.EnableWindow(FALSE);

		m_pLoadDataThread->m_bAutoDelete=FALSE;
		m_pLoadDataThread->ResumeThread();
	}
	return;
}

void CElementsView::LoadData()
{
	BeginLoadDataThread((LPVOID)this);
}

void CElementsView::ChangeDateGroupCtlUI(BOOL bEnable)
{
	for(UINT i=IDC_BTN_YEAR2; i<=IDC_BTN_HOUR2 ; i++)
		GetDlgItem(i)->EnableWindow(bEnable);

	GetDlgItem(IDC_DATETIME_ASK2)->EnableWindow(bEnable);
	m_ComboxCarSelect.EnableWindow(bEnable);

	m_DateTimeStart.EnableWindow(!bEnable);
	m_DateTimeEnd.EnableWindow(!bEnable);
}

void CElementsView::InitBtn()
{
	static BOOL bIsInit = FALSE;

	CButtonST* pBtn = NULL;
	for(UINT uId = IDC_BTN_YEAR2 ; uId <= IDC_BTN_HOUR2 ; uId++)
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