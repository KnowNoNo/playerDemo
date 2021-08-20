#include "stdafx.h"
#include "PublicFunction.h"


void Log(UINT nLevel, WCHAR* format, ...)
{
	TRACE("error");
}

void TcharToChar(TCHAR *tchar,char *_char)
{
	int nLength = WideCharToMultiByte(CP_ACP,0,tchar,-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,tchar,-1,_char,nLength,NULL,NULL);
	_char[nLength + 1] = '\0';
}

void CStringToChar(CString &csStr,char *_char)
{
	int nLength = WideCharToMultiByte(CP_ACP,0,csStr.GetBuffer(),-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,csStr.GetBuffer(),-1,_char,nLength,NULL,NULL);
	_char[nLength + 1] = '\0';
}

void CharToCString(char *_char,CString &csStr)
{
	int nLength = MultiByteToWideChar(CP_ACP,0,_char,-1,NULL,0);
	TCHAR *buf = new TCHAR[nLength+1];
	MultiByteToWideChar(CP_ACP,0,_char,-1,buf,nLength);
	buf[nLength] = '\0';
	csStr.Format(L"%s",buf);
	delete []buf;
}

int CStringToInt(CString &csStr)
{
	return _ttoi(csStr);
}

long CStringToLong(CString &csStr)
{
	return _ttol(csStr);
}

void CStringToTchar(CString &csStr,TCHAR *pTchar)
{
	_tcscpy(pTchar,csStr);
}

bool UTF8ToUnicode(char* pu8,CString &pun)  
{  
	// convert an UTF8 string to widechar   
	int utf8Len = strlen(pu8);
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  
	TCHAR *buf = new TCHAR[nLen+1];
	int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, buf, nLen);  
	buf[nLen] = 0;
	pun.Format(L"%s",buf);
	delete[] buf;
	return true;  
}  

//输入年月 得出多少天
int GetDayNumByYearMonth(int nYear,int nMonth)
{
	int nRet = 0;
	//	闰年
	if(nYear%4 == 0 && nYear%100 != 0 || nYear%400 == 0)
	{
		if(nMonth == 2)
			return 29;
	}
	switch(nMonth)
	{
		case	1:nRet = 31;break;
		case	2:nRet = 28;break;
		case	3:nRet = 31;break;
		case	4:nRet = 30;break;
		case	5:nRet = 31;break;
		case	6:nRet = 30;break;
		case	7:nRet = 31;break;
		case	8:nRet = 31;break;
		case	9:nRet = 30;break;
		case	10:nRet = 31;break;
		case	11:nRet = 30;break;
		case	12:nRet = 31;break;
	}

	return nRet;
}

BOOL DirectoryExist(CString Path)
{
	WIN32_FIND_DATA fd;
	BOOL ret = FALSE;
	HANDLE hFind = FindFirstFile(Path, &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		//目录存在
		ret = TRUE;

	}
	FindClose(hFind);
	return ret;
}
BOOL CreateDirectory(CString path)
{
	CString strCurPath;
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES);

	if(path.Find(L".") == -1 && path.Right(1) != "\\")
	{
		path += L"\\";
	}
	for(int nPos = 0 ,nBegin = 0; nPos < path.GetLength() ; nPos = nBegin+1)
	{
		nBegin = path.Find(L'\\',nPos);
		if(nBegin < 0)
			break;
		strCurPath = path.Left(nBegin);
		if( !DirectoryExist(strCurPath) && !::CreateDirectory( strCurPath, &attrib))
			return FALSE;
	}

	return TRUE;
}

CString GetDirFromPath(CString path)
{
	// 获取文件路径：
	CString strPath;
	int n=path.ReverseFind('\\')+1;
	strPath=path.Left(n);
	return strPath;
}

CString GetFileNameFormPath(CString path)
{
	// 获取文件名：

	CString strPath;
	int n=path.GetLength()-path.ReverseFind('\\')-1;
	strPath=path.Right(n);

	return strPath;
}

BOOL GetCarAndDateFromFileName(CString strFileName,CString &strCarId,CString &strDatetime,BOOL bIsVideoFile)
{
	int nDotPos = strFileName.Find(L'.');
	int nFirst_Pos = strFileName.Find(L'_');
	CString strTempDate;
	if(nDotPos < 0 || nFirst_Pos < 0)
		return FALSE;

	if(!bIsVideoFile)
	{
		int nSecond_Pos = strFileName.Find(L'_',nFirst_Pos+1);
		if(nSecond_Pos < 0)
			return FALSE;
		strTempDate = strFileName.Mid(nFirst_Pos+1,nSecond_Pos-nFirst_Pos-1);
	}
	strTempDate = strFileName.Mid(nFirst_Pos+1,nDotPos-nFirst_Pos-1);
	//	时间转化 "20010203040506" -> "2001-02-03 04:05:06"
	if( strTempDate.GetLength() !=  14)
		return FALSE;

	strTempDate.Insert(12,L':');
	strTempDate.Insert(10,L':');
	strTempDate.Insert(8,L' ');
	strTempDate.Insert(6,L'-');
	strTempDate.Insert(4,L'-');

	strCarId = strFileName.Left(nFirst_Pos);
	strDatetime = strTempDate;
	return TRUE;
}