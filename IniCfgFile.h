#pragma once

class CIniCfgFile : public CObject
{
public:	
	// ��ȡ���ַ�������
	CStringArray		m_strReadArray;
	// д����ַ�������
	CStringArray		m_strWriteArray;
	// ��־�ַ�������
	CStringArray		m_strTokenArray;
	// �ļ��Ƿ����
	BOOL				m_bFileExist;
	// �ļ�����
	CStdioFile			m_stdioFile;

public:
	// ���캯��
	CIniCfgFile();
	virtual				~CIniCfgFile();

	// ini �ļ���д����
	BOOL				WriteFile(CString strFileName);
	BOOL				ReadFile(CString strFileName);

	// ini �������ú���
	void				SetInt(CString strSection, CString strItem, int nValue);
	void				SetHex(CString strSection, CString strItem, DWORD nValue);
	void				SetString(CString strSection, CString strItem, CString strValue);

	// ini ������ȡ����
	int					GetInt(CString strSection, CString strItem, int nValue);
	//DWORD				GetHex(CString strSection, CString strItem, DWORD nValue);
	CString				GetString(CString strSection, CString strItem, CString strValue);
	//
	int					GetTokenString(CString strInput);
};
