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
	// ����ַ�������
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
	setlocale( LC_CTYPE, ("chs"));//������

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
		if( strLine.GetAt(0)==_T('[') )//����Section����һ������Ϊ[
		{
			str = strLine.Left( strLine.Find( _T("]") ) );//ȥ��]�ұ�
			str = str.Right( str.GetLength() - str.Find(_T("[")) -1 );//ȥ��[���
			str.TrimLeft();//����ַ���ͷ�ո�
			str.TrimRight();//����ַ���β�ո�

			if(strSection == str)//�ҵ�Section
			{
				while(i<nFileLines)
				{
					strLine = m_strWriteArray.GetAt(i++);
					strLine.TrimLeft();
					if(strLine.GetAt(0)==_T('['))//���������һ��[]�����Ҳ���Item
						break;				
					str = strLine.Left(strLine.Find(_T("=")));//ȥ��=�ұ�
					str.TrimLeft();
					str.TrimRight();
					if(strItem == str)//�ҵ�Item
					{
						strLine = strItem + _T("=") + strValue;
						m_strWriteArray[i-1] = strLine;
						return;
					}
				}
				//�Ҳ���Item
				strLine = strItem + _T("=") + strValue;
				m_strWriteArray.InsertAt(i-1, strLine);
				return;
			}
		}
	}
	//�Ҳ���Section
	//ֱ����������Section��Item��Value
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
	// �ļ������ڻ�������Ϊ��,ֱ�ӷ���Ĭ��ֵ
	if (m_bFileExist == false || m_strReadArray.GetSize()<=0 )
		return strValue;

	// ������
	int i = 0;
	int nFileLines = (int)m_strReadArray.GetSize();
	CString strLine, str;

	while(i<nFileLines)
	{
		strLine = m_strReadArray.GetAt(i++);
		strLine.TrimLeft();

		if(strLine.GetAt(0)==_T('[')) //����Section����һ������Ϊ[
		{
			str=strLine.Left(strLine.Find(_T("]")));//ȥ��]�ұ�
			str=str.Right(str.GetLength()-str.Find(_T("["))-1);//ȥ��[���
			str.TrimLeft();
			str.TrimRight();

			if(strSection == str)//�ҵ�Section
			{
				while(i<nFileLines)
				{
					strLine = m_strReadArray.GetAt(i++);
					strLine.TrimLeft();
					if(strLine.GetAt(0)==_T('['))
						return strValue;//���������һ��[]�����Ҳ���,����Ĭ��ֵ				
					str = strLine.Left(strLine.Find(_T("=")));//ȥ��=�ұ�
					str.TrimLeft();
					str.TrimRight();
					if(strItem == str)//�ҵ�Item
					{
						str=strLine.Right(strLine.GetLength()-strLine.Find(_T("="))-1);//ȥ��=���
						str.Trim();
						if (str.GetLength()>0)
							return str;
						else
							return strValue;
					}
				}
				return strValue;//�Ҳ���,����Ĭ��ֵ
			}
		}
	}
	return strValue;//�Ҳ���,����Ĭ��ֵ
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

