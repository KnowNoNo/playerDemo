#pragma once

class CIniCfgFile : public CObject
{
public:	
	// 读取的字符串队列
	CStringArray		m_strReadArray;
	// 写入的字符串队列
	CStringArray		m_strWriteArray;
	// 标志字符串队列
	CStringArray		m_strTokenArray;
	// 文件是否存在
	BOOL				m_bFileExist;
	// 文件对象
	CStdioFile			m_stdioFile;

public:
	// 构造函数
	CIniCfgFile();
	virtual				~CIniCfgFile();

	// ini 文件读写函数
	BOOL				WriteFile(CString strFileName);
	BOOL				ReadFile(CString strFileName);

	// ini 参数设置函数
	void				SetInt(CString strSection, CString strItem, int nValue);
	void				SetHex(CString strSection, CString strItem, DWORD nValue);
	void				SetString(CString strSection, CString strItem, CString strValue);

	// ini 参数读取函数
	int					GetInt(CString strSection, CString strItem, int nValue);
	//DWORD				GetHex(CString strSection, CString strItem, DWORD nValue);
	CString				GetString(CString strSection, CString strItem, CString strValue);
	//
	int					GetTokenString(CString strInput);
};
