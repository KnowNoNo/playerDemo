#pragma once
#include "StdAfx.h"
#include "EasySize.h"
#include "Oracle/DbOperation.h"
#include "map"
#include "BtnST.h"
// CElementsView 窗体视图

class CElementsView : public CFormView
{
	DECLARE_DYNCREATE(CElementsView)

public:
	CElementsView();           // 动态创建所使用的受保护的构造函数
	virtual ~CElementsView();
	
	DECLARE_EASYSIZE

public:
	enum { IDD = IDD_FORMVIEW_ELEMENTS };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	virtual void OnInitialUpdate();

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	// 表格设置
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
	BOOL	m_bColoredZones;
	BOOL	m_bCustomLines;
	BOOL	m_bTextObjects;
	BOOL	m_bDataTable;
	BOOL	m_bDataTableLegendKeys;
	BOOL	m_bXAxisReversed;

	BOOL	m_bInitUpdate;

	//	数据设置
	CButtonST	m_btnYear;
	CButtonST	m_btnMonth;
	CButtonST	m_btnDay;
	CButtonST	m_btnHour;

	int		m_nDateBtnSelect;
	CDateTimeCtrl   m_DateTimeAsk;

	//	查询设置
	CDateTimeCtrl   m_DateTimeStart;
	CDateTimeCtrl   m_DateTimeEnd;

	CComboBox		m_ComboxCarSelect;
	std::map<CString,BOOL>	m_mpCarSelect;

	//	数据查询	
	CAnalysisInfo	*m_pAnalyInfo;
	CWinThread		*m_pLoadDataThread;
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

	void InitChart();
	void UpdateChart();

	void InitBtn();

	BOOL TimeCheck();
	void ChangeDateGroupCtlUI(BOOL bEnable);

	void LoadData();
	void BeginLoadDataThread(LPVOID lpVoid);
	void UpdateComboxCtrl();

	afx_msg void OnUpdateData();
	afx_msg void OnUpdateChart();
	afx_msg void OnUpdateDateSelect(UINT uID);

	afx_msg void OnCbnSelchangeComboCarselect2();
	afx_msg void OnBnClickedBtnAsk2();
	afx_msg void OnDtnDatetimechangeDatetimeAsk2(NMHDR *pNMHDR, LRESULT *pResult);
};


