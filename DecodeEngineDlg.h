#pragma once


// DecodeEngineDlg �Ի���

class DecodeEngineDlg : public CDialog
{
	DECLARE_DYNAMIC(DecodeEngineDlg)

public:
	DecodeEngineDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DecodeEngineDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DECODER_ENGINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

private:
	int m_nType;
public:
	int GetDecodeType(){return m_nType;}
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
};
