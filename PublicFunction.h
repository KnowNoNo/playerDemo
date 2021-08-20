#pragma once


// ��Ч�ľ��
#define HANDLE_IS_VALID(h) ((HANDLE)(h) && ((HANDLE)(h)!=INVALID_HANDLE_VALUE))
// �رվ��
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

// ��־����
#define L_VERBOSE					0x00000000					// ��ϸ��һ���ǵ�����Ϣ
#define L_NORMAL					0x00000001					// ��������������ʱ��ʾ��һ����Ϣ����¼���й���
#define L_WARNING					0x00000002					// ���棬���������д��󣬵������������󣬳�����Ȼ���Լ���������ȥ
#define L_ERROR						0x00000003					// ���󣬳������г��������ش��󣬳������������ִֹ��
#define L_DEBUG						0x00000004					// ���ԣ������ڵ��԰����������־����Publish�����û����
#define L_LOGNUM					5							// ��־���������


void Log ( UINT nLevel, WCHAR* format, ...);

//unicode  ����ת��
void TcharToChar(TCHAR *tchar,char *_char);
void CStringToChar(CString &csStr,char *_char);//����
void CharToCString(char *_char,CString &csStr);
int  CStringToInt(CString &csStr);//����
long CStringToLong(CString &csStr);
void CStringToTchar(CString &csStr,TCHAR *pTchar);
bool UTF8ToUnicode(char* pu8,CString &pun); //����

enum
{
	FILEICON,
	DIRECTORYICON,
	LOCFILE,
	SERVFILE
};

//�������� �ó�������
int GetDayNumByYearMonth(int nYear,int nMonth);


BOOL DirectoryExist(CString Path);
BOOL CreateDirectory(CString path);

//

CString GetDirFromPath(CString path);
CString GetFileNameFormPath(CString path);

//	�ļ�����ʽ ��Ƶ��5001_20010203040506.mp4 ��ͼ: 5001_20010203040506_000.jpg
BOOL GetCarAndDateFromFileName(CString strFileName,CString &strCarId,CString &strDatetime,BOOL bIsVideoFile = TRUE);			
