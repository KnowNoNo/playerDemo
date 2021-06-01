#pragma once
#include "StdAfx.h"

// CAnalysisView 窗体视图

class CAnalysisView : public CFormView
{
	DECLARE_DYNCREATE(CAnalysisView)

protected:
	CAnalysisView();           // 动态创建所使用的受保护的构造函数
	virtual ~CAnalysisView();

public:
	enum { IDD = IDD_FORMVIEW_ANALYSIS1 };
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
public:
	// 时间设置
	CDateTimeCtrl m_DateTimeStart;
	CDateTimeCtrl m_DateTimeAsk;
	// 表格设置
	CBCGPChartCtrl	m_wndChartYear;
	//CBCGPChartCtrl	m_wndChartMonth;
	//CBCGPChartCtrl	m_wndChartDay;
	//CBCGPChartCtrl	m_wndChartHour;
	//CBCGPButton	m_wndDisplayShadow;
	CBCGPComboBox	m_wndDataLabelPosition;
	int		m_nFillGradientType;
	BOOL	m_bShowDataLabels;
	int		m_nDataLabelPosition;
	BOOL	m_bDisplayShadow;
	int		m_nColorType;

public:
	void InitChart();
	void UpdateChart(CBCGPChartVisualObject* pChart);
	
	afx_msg void OnUpdateChart();
	DECLARE_MESSAGE_MAP()
};


