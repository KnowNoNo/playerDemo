#include "stdafx.h"
#include "CarInfo.h"
#include "./Oracle/DbOperation.h"
#include "PlayDemo.h"

UINT ThreadProc_LoadData(LPVOID lParam)
{
	CAnalysisInfo	*pAnalyInfo = (CAnalysisInfo*)lParam;
	CString dbFilePath = pAnalyInfo->strDbFilePath;

	CDbOperation dbOperator;

	//	如果CDbOperation在线程中，则必须在此线程中CoInitialize(NULL)
	CoInitialize(NULL);

	pAnalyInfo->mpCarInfo.clear();
	//	获取数据库数据

	dbOperator.GetAllData(dbFilePath,pAnalyInfo->tmStart,pAnalyInfo->tmEnd);

	CString carName,picPath,videoPath;
	COleDateTime date;
	//	分析数据库数据
	while (!dbOperator.RecordSet.IsEOF())
	{
		carName = dbOperator.RecordSet.GetValueString(L"CAR_NAME");
		date    = dbOperator.RecordSet.GetValueDate(L"HAPPEN_DATE");
		if(date.GetStatus() != 0)
			continue;
		picPath = dbOperator.RecordSet.GetValueString(L"PIC_PATH");
		videoPath = dbOperator.RecordSet.GetValueString(L"VIDEO_PATH");

		if(pAnalyInfo->mpCarInfo.count(carName) < 1)
		{
			pAnalyInfo->mpCarInfo.insert(std::map<CString,CCarInfo*>::value_type(carName,new CCarInfo));
		}
		pAnalyInfo->mpCarInfo[carName]->UpdateData(date.GetYear(),
			date.GetMonth(),date.GetDay(),date.GetHour()+1,picPath,videoPath);
		dbOperator.RecordSet.MoveNext();
	}

	pAnalyInfo->Unlock();
	return 0;
}