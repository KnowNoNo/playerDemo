#pragma once
#include "map"

// CDlgCarSelect �Ի���

class CDlgCarSelect : public CDialog
{
	DECLARE_DYNAMIC(CDlgCarSelect)

public:
	CDlgCarSelect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCarSelect();

// �Ի�������
	enum { IDD = IDD_DIALOG_CARSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:

	void		InitData(std::map<CString,int>* pCarList);
protected:	
	CListCtrl				m_list;
	std::map<CString,BOOL>*	m_pCarList;

	void		AddData();
protected:
	virtual void OnOK();
public:
	afx_msg void OnNMClickListCar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListCar(NMHDR *pNMHDR, LRESULT *pResult);
};
