#include "stdafx.h"
#include "IniCfgFile.h"
#include <locale.h>

CIniCfgFile::CIniCfgFile()
{
	m_bFileExist = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
CIniCfgFile::~CIniCfgFile()
{
	// 清除字符串队列
	if (m_strTokenArray.GetSize()>0)
		m_strTokenArray.RemoveAll();
	if(m_strReadArray.GetSize()>0)
		m_strReadArray.RemoveAll();
	if(m_strWriteArray.GetSize()>0)
		m_strWriteArray.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
BOOL CIniCfgFile::ReadFile(CString strFileName)
{
	setlocale( LC_CTYPE, ("chs"));//添加这个

	m_bFileExist = m_stdioFile.Open(strFileName, CFile::modeRead);
	if (!m_bFileExist)
	{
		return FALSE;
	}

	CString strLine;
	m_strReadArray.RemoveAll();
	while (m_stdioFile.ReadString(strLine))
	{
		m_strReadArray.Add(strLine);
	}
	m_stdioFile.Close();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL CIniCfgFile::WriteFile(CString strFileName)
{
	CStdioFile stdFileOut;
	BOOL bFileOut = stdFileOut.Open(strFileName, CFile::modeCreate | CFile::modeWrite);

	if(!bFileOut)
		return FALSE;

	CString strLine;
	for(int i = 0; i<m_strWriteArray.GetSize(); i++)
	{
		strLine = m_strWriteArray[i];
		stdFileOut.WriteString(strLine+_T("\n"));
	}
	stdFileOut.Close();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void CIniCfgFile::SetInt(CString strSection, CString strItem, int nValue)
{
	CString strTemp;
	strTemp.Format(_T("%d"), nValue);
	SetString(strSection, strItem, strTemp);
}

///////////////////////////////////////////////////////////////////////////////
void CIniCfgFile::SetHex(CString strSection, CString strItem, DWORD nValue)
{
	CString strTemp;
	strTemp.Format(_T("0x%X"), nValue);
	SetString(strSection, strItem, strTemp);
}

///////////////////////////////////////////////////////////////////////////////
void CIniCfgFile::SetString(CString strSection, CString strItem, CString strValue)
{
	int i = 0;
	int nFileLines = (int) m_strWriteArray.GetSize();
	CString strLine,str;

	while(i<nFileLines)
	{
		strLine = m_strWriteArray.GetAt(i++);
		strLine.TrimLeft();
		if( strLine.GetAt(0)==_T('[') )//查找Section，第一个必须为[
		{
			str = strLine.Left( strLine.Find( _T("]") ) );//去掉]右边
			str = str.Right( str.GetLength() - str.Find(_T("[")) -1 );//去掉[左边
			str.TrimLeft();//清除字符串头空格
			str.TrimRight();//清除字符串尾空格

			if(strSection == str)//找到Section
			{
				while(i<nFileLines)
				{
					strLine = m_strWriteArray.GetAt(i++);
					strLine.TrimLeft();
					if(strLine.GetAt(0)==_T('['))//如果到达下一个[]，即找不到Item
						break;				
					str = strLine.Left(strLine.Find(_T("=")));//去掉=右边
					str.TrimLeft();
					str.TrimRight();
					if(strItem == str)//找到Item
					{
						strLine = strItem + _T("=") + strValue;
						m_strWriteArray[i-1] = strLine;
						return;
					}
				}
				//找不到Item
				strLine = strItem + _T("=") + strValue;
				m_strWriteArray.InsertAt(i-1, strLine);
				return;
			}
		}
	}
	//找不到Section
	//直接在最后加入Section，Item，Value
	m_strWriteArray.Add(_T("[") + strSection + _T("]"));
	m_strWriteArray.Add(strItem + _T("=") + strValue);
	return;
}

/////////////////////////////////////////////////////////////////////////////
int CIniCfgFile::GetInt(CString strSection, CString strItem, int nValue)
{
	CString strTemp;
	strTemp.Format(_T("%d"),nValue);
	return _wtoi(GetString(strSection, strItem, strTemp));
}

///////////////////////////////////////////////////////////////////////////////
//DWORD CIniCfgFile::GetHex(CString strSection, CString strItem, DWORD nValue)
//{
//	CString strTemp;
//	strTemp.Format(_T("0x%x"),nValue);
//	CString strHex = GetString(strSection, strItem, strTemp);
//	DWORD nRetValue = 0;
//	sscanf_s(strHex, _T("%x"), &nRetValue);
//	return nRetValue;	 
//}

///////////////////////////////////////////////////////////////////////////////
CString CIniCfgFile::GetString(CString strSection, CString strItem, CString strValue)
{
	// 文件不存在或者内容为空,直接返回默认值
	if (m_bFileExist == false || m_strReadArray.GetSize()<=0 )
		return strValue;

	// 处理部分
	int i = 0;
	int nFileLines = (int)m_strReadArray.GetSize();
	CString strLine, str;

	while(i<nFileLines)
	{
		strLine = m_strReadArray.GetAt(i++);
		strLine.TrimLeft();

		if(strLine.GetAt(0)==_T('[')) //查找Section，第一个必须为[
		{
			str=strLine.Left(strLine.Find(_T("]")));//去掉]右边
			str=str.Right(str.GetLength()-str.Find(_T("["))-1);//去掉[左边
			str.TrimLeft();
			str.TrimRight();

			if(strSection == str)//找到Section
			{
				while(i<nFileLines)
				{
					strLine = m_strReadArray.GetAt(i++);
					strLine.TrimLeft();
					if(strLine.GetAt(0)==_T('['))
						return strValue;//如果到达下一个[]，即找不到,返回默认值				
					str = strLine.Left(strLine.Find(_T("=")));//去掉=右边
					str.TrimLeft();
					str.TrimRight();
					if(strItem == str)//找到Item
					{
						str=strLine.Right(strLine.GetLength()-strLine.Find(_T("="))-1);//去掉=左边
						str.Trim();
						if (str.GetLength()>0)
							return str;
						else
							return strValue;
					}
				}
				return strValue;//找不到,返回默认值
			}
		}
	}
	return strValue;//找不到,返回默认值
}

///////////////////////////////////////////////////////////////////////////////
int	CIniCfgFile::GetTokenString(CString strInput)
{
	if (m_strTokenArray.GetSize()>0)
		m_strTokenArray.RemoveAll();

	int nCount = 0;

	CString resToken;
	int curPos= 0;
	do
	{
		resToken= strInput.Tokenize(_T(","), curPos).Trim();
		if(resToken != _T(""))
		{
			m_strTokenArray.Add(resToken); 
			nCount++;
		}
	} while(resToken != _T(""));

	return nCount;
}

