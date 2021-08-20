#pragma once
#include "StdAfx.h"
#include "EasySize.h"
#include "Oracle/DbOperation.h"
#include "map"
#include "BtnST.h"
// CAnalysisView 窗体视图

class CAnalysisView : public CFormView
{
	DECLARE_DYNCREATE(CAnalysisView)
  
public:
	CAnalysisView();           // 动态创建所使用的受保护的构造函数
	virtual ~CAnalysisView();

	DECLARE_EASYSIZE

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
	
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
public:
	// 表格设置
	CBCGPChartCtrl	m_wndChart;
	CBCGPComboBox	m_wndDataLabelPosition;
	
	int		m_nFillGradientType;
	BOOL	m_bShowDataLabels;
	int		m_nDataLabelPosition;
	BOOL	m_bDisplayShadow;
	int		m_nColorType;
	BOOL    m_bInitUpdate;

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

	afx_msg void OnCbnSelchangeCombo1Carselect();
	afx_msg void OnBnClickedBtnAsk();

	afx_msg void OnDtnDatetimechangeDatetime1Ask(NMHDR *pNMHDR, LRESULT *pResult);
};


