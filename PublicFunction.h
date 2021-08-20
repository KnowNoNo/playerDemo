#pragma once


// 有效的句柄
#define HANDLE_IS_VALID(h) ((HANDLE)(h) && ((HANDLE)(h)!=INVALID_HANDLE_VALUE))
// 关闭句柄
#define CLOSE_HANDLE(h)\
{\
	if ( HANDLE_IS_VALID(h) )\
	{\
	::CloseHandle ( h );\
	(h) = NULL;\
	}\
}

#define CLOSE_FILE_HANDLE(h)\
{\
	if ( HANDLE_IS_VALID(h->m_hFile) )\
	{\
	h->Close();\
	delete h;\
	(h) = NULL;\
	}\
}
#define SLEEP_RETURN(x)\
{\
	;}
//if ( ::WaitForSingleObject ( hEvtEndModule, x ) == WAIT_OBJECT_0 )\
//{\
//CLOSE_FILE_HANDLE(m_pFile);\
//return FALSE;\
//}\
}

// 日志级别
#define L_VERBOSE					0x00000000					// 详细，一般是调试信息
#define L_NORMAL					0x00000001					// 正常，程序运行时显示的一般信息，纪录运行过程
#define L_WARNING					0x00000002					// 警告，程序运行有错误，但不是致命错误，程序仍然可以继续运行下去
#define L_ERROR						0x00000003					// 错误，程序运行出现了严重错误，程序必须立即终止执行
#define L_DEBUG						0x00000004					// 调试，程序在调试版里输出的日志，在Publish版里就没有了
#define L_LOGNUM					5							// 日志级别的数量


void Log ( UINT nLevel, WCHAR* format, ...);

//unicode  类型转化
void TcharToChar(TCHAR *tchar,char *_char);
void CStringToChar(CString &csStr,char *_char);//可用
void CharToCString(char *_char,CString &csStr);
int  CStringToInt(CString &csStr);//可用
long CStringToLong(CString &csStr);
void CStringToTchar(CString &csStr,TCHAR *pTchar);
bool UTF8ToUnicode(char* pu8,CString &pun); //可用

enum
{
	FILEICON,
	DIRECTORYICON,
	LOCFILE,
	SERVFILE
};

//输入年月 得出多少天
int GetDayNumByYearMonth(int nYear,int nMonth);


BOOL DirectoryExist(CString Path);
BOOL CreateDirectory(CString path);

//

CString GetDirFromPath(CString path);
CString GetFileNameFormPath(CString path);

//	文件名格式 视频：5001_20010203040506.mp4 截图: 5001_20010203040506_000.jpg
BOOL GetCarAndDateFromFileName(CString strFileName,CString &strCarId,CString &strDatetime,BOOL bIsVideoFile = TRUE);			
