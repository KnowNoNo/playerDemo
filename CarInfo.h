#pragma once

//#include <shlwapi.h>

//#pragma comment(lib,"Shlwapi.lib") //���û�����У������link����

#define		INIT_STOREINFO_DATETIME			CTime(2020,1,1,0,0,0)
//#define		MAX_CAR_COUNT					50
#define		MAX_CAR_YEAR_COUNT				50
#define		MAX_CAR_MONTH_COUNT				12
#define		MAX_CAR_DAY_COUNT				31
#define		MAX_CAR_HOUR_COUNT				24

//
UINT ThreadProc_LoadAnalyViewData(LPVOID lParam);
UINT ThreadProc_LoadElementViewData(LPVOID lParam);

class CCarInfo
{
public:
	int			nID;													// ����ID / ip
	CString		strPath;												// �ļ�·��
	BOOL		bIsInit;												// ��ʼ��
	int			nDefaultInitYear;										// Ĭ�����ݳ�ʼ�� �������
	int			arrInfo[MAX_CAR_YEAR_COUNT + 1][MAX_CAR_MONTH_COUNT + 1][MAX_CAR_DAY_COUNT + 1][MAX_CAR_HOUR_COUNT + 1];
	//CString		*arrPicPath;											//	ͼƬ·��
	//CString		*arrVideoPath;											//	��Ƶ·��
	CCarInfo()
	{
		nID = -1;
		Clear();
		nDefaultInitYear = (INIT_STOREINFO_DATETIME.GetYear()) - 1;
		//arrPicPath = NULL;
		//arrPicPath = NULL;
		//InitData(strP);
	}
	~CCarInfo()
	{
		//if( arrPicPath )
		//	delete arrPicPath;
		//if( arrVideoPath )
		//	delete arrVideoPath;
	}

	//void init

	void Clear()
	{
		memset(arrInfo,0,sizeof(arrInfo));
	}
	
	//	�£�1~12  �죺1~31  Сʱ��1~24 
	void UpdateData(int nYear, int nMonth, int nDay, int nHour, CString picPath, CString videoPath)
	{
		if(nYear < nDefaultInitYear)
			return;
		arrInfo[nYear-nDefaultInitYear][nMonth][nDay][nHour]++;
		//
		arrInfo[nYear-nDefaultInitYear][nMonth][nDay][0]++;
		arrInfo[nYear-nDefaultInitYear][nMonth][0][0]++;
		arrInfo[nYear-nDefaultInitYear][0][0][0]++;
		arrInfo[0][0][0][0]++;
		//WriteDateFile(nYear,nMonth,nDay,nHour);
	}
	//	�£�1~12  �죺1~31  Сʱ��1~24 
	int GetData(int nYear = 0, int nMonth = 0, int nDay = 0, int nHour = 0)
	{
		return arrInfo[nYear-nDefaultInitYear][nMonth][nDay][nHour];
	}

	//void ReadDateFile()
	//{
	//	CFile	file;
	//	if( !file.Open(strPath,CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	//	{
	//		TRACE(_T("File could not be opened \n"));
	//		return;
	//	}

	//	file.Read((char *)&arrInfo, sizeof(arrInfo));  //һֱ�����ļ�����

	//	file.Close();
	//}

	//void WriteDateFile(int nYear, int nMonth, int nDay, int nHour)
	//{
	//	CFile	file;
	//	if( !file.Open(strPath,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone) )
	//	{
	//		TRACE(_T("File could not be opened \n"));
	//		return;
	//	}

	//	nYear -= nDefaultInitYear;

	//	int nPos = (char*)(&arrInfo[nYear][nMonth][nDay][nHour]) - (char*)(&arrInfo[0][0][0][0]);
	//	file.Seek(nPos,CFile::begin);
	//	file.Write((char *)(&arrInfo[nYear][nMonth][nDay][nHour]), sizeof(int));  //һֱ�����ļ����� 

	//	file.Close();
	//}

	////	��ʼ������
	//void InitData(CString strPath)
	//{
	//	bIsInit = TRUE;
	//	//	�ӷ�����down����

	//	if ( !PathFileExists(strPath) )
	//	{
	//		//	
	//		AfxMessageBox(L"����������ʧ��");
	//	}
	//	else
	//	{
	//		ReadDateFile();
	//	}
	//}
};
