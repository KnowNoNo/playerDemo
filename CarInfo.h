#pragma once

//#include <shlwapi.h>

//#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误

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
	int			nID;													// 车辆ID / ip
	CString		strPath;												// 文件路径
	BOOL		bIsInit;												// 初始化
	int			nDefaultInitYear;										// 默认数据初始化 所在年份
	int			arrInfo[MAX_CAR_YEAR_COUNT + 1][MAX_CAR_MONTH_COUNT + 1][MAX_CAR_DAY_COUNT + 1][MAX_CAR_HOUR_COUNT + 1];
	//CString		*arrPicPath;											//	图片路径
	//CString		*arrVideoPath;											//	视频路径
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
	
	//	月：1~12  天：1~31  小时：1~24 
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
	//	月：1~12  天：1~31  小时：1~24 
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

	//	file.Read((char *)&arrInfo, sizeof(arrInfo));  //一直读到文件结束

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
	//	file.Write((char *)(&arrInfo[nYear][nMonth][nDay][nHour]), sizeof(int));  //一直读到文件结束 

	//	file.Close();
	//}

	////	初始化数据
	//void InitData(CString strPath)
	//{
	//	bIsInit = TRUE;
	//	//	从服务器down数据

	//	if ( !PathFileExists(strPath) )
	//	{
	//		//	
	//		AfxMessageBox(L"服务器连接失败");
	//	}
	//	else
	//	{
	//		ReadDateFile();
	//	}
	//}
};
