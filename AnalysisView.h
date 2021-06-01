#pragma once
#include "StdAfx.h"

// CAnalysisView ������ͼ

class CAnalysisView : public CFormView
{
	DECLARE_DYNCREATE(CAnalysisView)

protected:
	CAnalysisView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


public:
	virtual void OnInitialUpdate();
public:
	// ʱ������
	CDateTimeCtrl m_DateTimeStart;
	CDateTimeCtrl m_DateTimeAsk;
	// �������
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


