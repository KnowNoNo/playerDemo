// ElementsView.cpp : 实现文件
//

#include "stdafx.h"
#include "PlayDemo.h"
#include "ElementsView.h"


// CElementsView

IMPLEMENT_DYNCREATE(CElementsView, CFormView)

CElementsView::CElementsView()
	: CFormView(CElementsView::IDD)
{
	m_bXAxis = TRUE;
	m_bXAxisName = TRUE;
	m_bYAxis = TRUE;
	m_bYAxisName = TRUE;
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
	m_bSecondSeries = TRUE;
	m_bColoredZones = FALSE;
	m_bCustomLines = TRUE;
	m_bTextObjects = FALSE;
	m_bDataTable = TRUE;
	m_bDataTableLegendKeys = TRUE;
	m_bXAxisReversed = FALSE;

	m_strokeStyle.SetDashStyle(CBCGPStrokeStyle::BCGP_DASH_STYLE_DASH_DOT);
}

CElementsView::~CElementsView()
{
}

void CElementsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHART, m_wndChart);
	DDX_Check(pDX, IDC_CHART_CUSTOM_LINES, m_bCustomLines);
	DDX_Check(pDX, IDC_CHART_SECOND_SERIES, m_bSecondSeries);
	DDX_Check(pDX, IDC_DATA_TABLE, m_bDataTable);
	DDX_Check(pDX, IDC_CHART_AXIS_X_INTERVAL_INTERLASING, m_bXIntervalInterlasing);
	DDX_Check(pDX, IDC_CHART_AXIS_Y_INTERVAL_INTERLASING, m_bYIntervalInterlasing);
	DDX_Check(pDX, IDC_CHART_AXIS_Y_GRID_LINES, m_bYAxisGridLines);
	DDX_Check(pDX, IDC_CHART_AXIS_X_GRID_LINES, m_bXAxisGridLines);
}

BEGIN_MESSAGE_MAP(CElementsView, CFormView)
	ON_BN_CLICKED(IDC_CHART_CUSTOM_LINES, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_SECOND_SERIES, OnUpdateChart)
	ON_BN_CLICKED(IDC_DATA_TABLE, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_AXIS_X_INTERVAL_INTERLASING, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_AXIS_Y_INTERVAL_INTERLASING, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_AXIS_Y_GRID_LINES, OnUpdateChart)
	ON_BN_CLICKED(IDC_CHART_AXIS_X_GRID_LINES, OnUpdateChart)
END_MESSAGE_MAP()


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
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->SetChartTitle(_T("Chart Title"));

	CBCGPChartSeries* pSeries1 = pChart->CreateSeries(_T("Series 1"));
	pSeries1->SetCurveType (BCGPChartFormatSeries::CCT_SPLINE);

	CBCGPChartSeries* pSeries2 = pChart->CreateSeries(_T("Series 2"));
	pSeries2->SetCurveType (BCGPChartFormatSeries::CCT_SPLINE_HERMITE);

	pSeries1->AddDataPoint(0.);
	pSeries1->AddDataPoint(2.);
	pSeries1->AddDataPoint(5.);
	pSeries1->AddDataPoint(3.);
	pSeries1->AddDataPoint(3.5);
	pSeries1->AddDataPoint(5.);
	pSeries1->AddDataPoint(8.);
	pSeries1->AddDataPoint(7.);

	pSeries2->AddDataPoint(2.);
	pSeries2->AddDataPoint(4.);
	pSeries2->AddDataPoint(6.);
	pSeries2->AddDataPoint(7.);
	pSeries2->AddDataPoint(6.);
	pSeries2->AddDataPoint(3.);
	pSeries2->AddDataPoint(1.);
	pSeries2->AddDataPoint(0.);
	
	m_brFill1 = CBCGPBrush(CBCGPColor::RoyalBlue, .2);
	m_brFill2 = CBCGPBrush(CBCGPColor::Peru, .2);
	m_brFill3 = CBCGPBrush(CBCGPColor::LightSkyBlue, CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT, .4);
	m_brFill4 = CBCGPBrush(CBCGPColor::LightYellow);
	m_brLine1 = CBCGPBrush(CBCGPColor::DarkRed);
	m_brLine2 = CBCGPBrush(CBCGPColor::DeepSkyBlue);
	m_brText1 = CBCGPBrush(CBCGPColor::SteelBlue);
	m_brText2 = CBCGPBrush(CBCGPColor::Brown);

	pChart->SetSeriesShadow();

	GetParent()->SetWindowText(L"数据分析2.0");

	OnUpdateChart();
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

	pChart->ShowSeries(m_bSecondSeries, 1);

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
