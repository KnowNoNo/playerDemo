#pragma once


#ifdef _DEBUG
// CCmdDialog �Ի���

class CCmdDialog : public CDialog
{
	DECLARE_DYNAMIC(CCmdDialog)

public:
	CCmdDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCmdDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();

private:
	void HandleMessage();
	CString	m_strCommand;
};

#endif