// AnalysisView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlayDemo.h"
#include "AnalysisView.h"


// CAnalysisView

IMPLEMENT_DYNCREATE(CAnalysisView, CFormView)

CAnalysisView::CAnalysisView()
	: CFormView(CAnalysisView::IDD)
{
	m_nFillGradientType = 0;
	m_bShowDataLabels = TRUE;
	m_nDataLabelPosition = 0;
	m_bDisplayShadow = TRUE;
	m_nColorType = 0;

}

CAnalysisView::~CAnalysisView()
{
}

void CAnalysisView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEP_START, m_DateTimeStart);
	DDX_Control(pDX, IDC_DATETIME_ASK, m_DateTimeAsk);
	//DDX_Control(pDX, IDC_SERIES_SHADOW, m_wndDisplayShadow);
	DDX_Control(pDX, IDC_DATA_LABEL_POSITION, m_wndDataLabelPosition);
	DDX_Control(pDX, IDC_CHART_YEAR, m_wndChartYear);
	DDX_Control(pDX, IDC_CHART_MONTH, m_wndChartMonth);
	DDX_Control(pDX, IDC_CHART_DAY, m_wndChartDay);
	DDX_Control(pDX, IDC_CHART_HOUR, m_wndChartHour);
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

END_MESSAGE_MAP()


// CAnalysisView ���

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


// CAnalysisView ��Ϣ��������

void CAnalysisView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ�����ר�ô����/����û���
	GetParent()->SetWindowText(L"���ݷ���");

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

	CBCGPChartVisualObject* pChartY = m_wndChartYear.GetChart();
	ASSERT_VALID(pChartY);

	CBCGPChartVisualObject* pChartM = m_wndChartMonth.GetChart();
	ASSERT_VALID(pChartM);

	CBCGPChartVisualObject* pChartD = m_wndChartDay.GetChart();
	ASSERT_VALID(pChartD);

	CBCGPChartVisualObject* pChartH = m_wndChartHour.GetChart();
	ASSERT_VALID(pChartH);

	CTime timeS,timeA;
	CString csTime;
	timeS = DATETIME_START_INIT;
	timeA = CTime::GetCurrentTime();
	if(timeA.GetTime() < timeS.GetTime())
		timeA = timeS;

	m_DateTimeStart.SetTime(&timeS);
	m_DateTimeAsk.SetTime(&timeA);

	csTime.Format(L"%d ~ %d ��",timeS.GetYear(),timeA.GetYear());
	
	InitChart(pChartY,csTime);
	
	CBCGPChartBarSeries* pBarSeries1 = DYNAMIC_DOWNCAST(CBCGPChartBarSeries, pChartY->CreateSeries(_T("Fuel")));

	pBarSeries1->AddDataPoint(L"2", 1);
	pBarSeries1->AddDataPoint(L"2", 2);
	pBarSeries1->AddDataPoint(L"3", 3);
		pBarSeries1->AddDataPoint(L"3", 3);	pBarSeries1->AddDataPoint(L"3", 3);	pBarSeries1->AddDataPoint(L"3", 3);	pBarSeries1->AddDataPoint(L"3", 3);	pBarSeries1->AddDataPoint(L"3", 3);	pBarSeries1->AddDataPoint(L"3", 3);
	////pBarSeries1->AddDataPoint(L"4", 0);
	//pBarSeries1->AddDataPoint(L"5", 100);

	pBarSeries1->SetGroupID(0);
	//csTime.Format(L"%d",timeA.)
	UpdateChart(pChartY);

}

void CAnalysisView::InitChart(CBCGPChartVisualObject* pChart,CString csTitle)
{
	BCGPChartCategory category = BCGPChartColumn; //
	BCGPChartType type = BCGP_CT_SIMPLE;

	pChart->SetChartType(category, type);
	pChart->SetChartTitle(csTitle);//
}

void CAnalysisView::OnUpdateChart()
{
	UpdateData();

	m_wndDataLabelPosition.EnableWindow(m_bShowDataLabels);

	UpdateChart(m_wndChartYear.GetChart());
	UpdateChart(m_wndChartMonth.GetChart());
	UpdateChart(m_wndChartDay.GetChart());
	UpdateChart(m_wndChartHour.GetChart());
	
}

void CAnalysisView::UpdateChart(CBCGPChartVisualObject* pChart)
{
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