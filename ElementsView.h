#pragma once



// CElementsView ������ͼ

class CElementsView : public CFormView
{
	DECLARE_DYNCREATE(CElementsView)

protected:
	CElementsView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CElementsView();

public:
	enum { IDD = IDD_FORMVIEW_ELEMENTS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	CBCGPChartCtrl	m_wndChart;
	BOOL	m_bXAxis;
	BOOL	m_bXAxisName;
	BOOL	m_bYAxis;
	BOOL	m_bYAxisName;
	BOOL	m_bDataLabels;
	BOOL	m_bDataMarkers;
	BOOL	m_bLegend;
	BOOL	m_bLegendOverlapsChart;
	BOOL	m_bTitle;
	BOOL	m_bTitleOverlapsChart;
	BOOL	m_bXIntervalInterlasing;
	BOOL	m_bYIntervalInterlasing;
	BOOL	m_bYAxisGridLines;
	BOOL	m_bXAxisGridLines;
	BOOL	m_bSecondSeries;
	BOOL	m_bColoredZones;
	BOOL	m_bCustomLines;
	BOOL	m_bTextObjects;
	BOOL	m_bDataTable;
	BOOL	m_bDataTableLegendKeys;
	BOOL	m_bXAxisReversed;

protected:
	CBCGPBrush			m_brFill1;
	CBCGPBrush			m_brFill2;
	CBCGPBrush			m_brFill3;
	CBCGPBrush			m_brFill4;
	CBCGPBrush			m_brLine1;
	CBCGPBrush			m_brLine2;
	CBCGPBrush			m_brText1;
	CBCGPBrush			m_brText2;
	CBCGPStrokeStyle	m_strokeStyle;

public:
	afx_msg void OnUpdateChart();
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
};


