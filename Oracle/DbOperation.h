#pragma once

#include "ADOConnection.h"
#include "ADORecordset.h"
#include "vector"
#include "../PublicFunction.h"

//	如果CDbOperation在线程中，则必须在此线程中CoInitialize(NULL)

class CDbOperation : public CObject
{
public:
	CDbOperation(void);
	~CDbOperation(void);

public:
	
public:
	CString			m_strUdlPath;
	CString			m_strTabName;
	CADOConnection  DBConnection;  //数据连接对象
	CADORecordset   RecordSet;     //数据集对象

public:
	BOOL OpenDB(CString path,CString sqlCmd)
	{
		//synch::TMutex::TAutoLock lock(this->m_sqlLock);
		BOOL succ = TRUE;
		if (!DBConnection.IsOpen())
		{
			//udlPath = path;
			CString connStr = L"File Name=";
			connStr += path;
			DBConnection.m_strConnect = connStr;
			m_strUdlPath = connStr;
			succ &= DBConnection.Open(L"");
		}
		char chSql[500];
		CStringToChar(sqlCmd,chSql);
		succ &= RecordSet.Open(chSql, DBConnection.m_pConnection);
		return succ;
	}
	void CloseDB()
	{
		//synch::TMutex::TAutoLock lock(this->m_sqlLock);
		if (DBConnection.IsOpen())
		{
			DBConnection.Close();
		}
	}
	BOOL IsOpen()
	{
		//synch::TMutex::TAutoLock lock(this->m_sqlLock);
		//return (this->DBConnection.IsOpen() == TRUE);
		//************************************
		return DBConnection.IsOpen();
	
		//************************************
	}
	
	bool GetAllData(CString path,CTime tmStart,CTime tmEnd)
	{
		CString sql;
		sql.Format(L"select * from T_PLJS_JCBJ where HAPPEN_DATE >= to_date( '%s', 'YYYY-MM-DD hh24:mi:ss' ) AND HAPPEN_DATE <= to_date( '%s', 'YYYY-MM-DD hh24:mi:ss' )"
			,tmStart.Format(L"%Y-%m-%d %H:%M:%S") , tmEnd.Format(L"%Y-%m-%d %H:%M:%S"));
		if(OpenDB(path,sql))
		{
			return true;
		}
		return false;
	}

	bool GetCarData(CString path,CTime tmStart,CTime tmEnd,CString strCarId)
	{
		CString sql;
		sql.Format(L"select * from T_PLJS_JCBJ where CAR_NAME = %s AND HAPPEN_DATE >= to_date( '%s', 'YYYY-MM-DD hh24:mi:ss' ) AND HAPPEN_DATE <= to_date( '%s', 'YYYY-MM-DD hh24:mi:ss' )"
			,strCarId,tmStart.Format(L"%Y-%m-%d %H:%M:%S") , tmEnd.Format(L"%Y-%m-%d %H:%M:%S"));
		if(OpenDB(path,sql))
		{
			return true;
		}
		return false;
	}

	bool GetCarName(CString path)
	{
		CString sql;
		sql = L"select distinct CAR_NAME from T_PLJS_JCBJ order by CAR_NAME";
		if(OpenDB(path,sql))
		{
			return true;
		}
		return false;
	}

	bool IsExistTable(CString path,CString tabName)
	{
		CString		  sql;
		
		sql.Format(L"select count(*) from user_tables t where table_name= upper('%s')",tabName);
		if(!OpenDB(path,sql))
		{
			return false;
		}
		
		_variant_t TheValue;
		TheValue = RecordSet.GetCollect();
		
		return (TheValue.lVal>0);
	}

	bool InitDatabase(CString path)
	{
		if(!IsExistTable(path,L"T_PLJS_JCBJ"))
		{
			if(!OpenDB(path,"CREATE TABLE T_PLJS_JCBJ(CAR_NAME VARCHAR(20),HAPPEN_DATE DATE, PIC_PATH VARCHAR(256) ,VIDEO_PATH VARCHAR(256))"))
				return false;
		}
		return true;
	}

	bool InsertDataPic(CString path,CString locNo,CString date,CString picPath)
	{
		CString sql;
		sql.Format(L"insert into T_PLJS_JCBJ(CAR_NAME,HAPPEN_DATE,PIC_PATH) values('%s',to_date( '%s', 'YYYY/MM/DD hh24:mi:ss' ),'%s')",locNo,date,picPath);
		if(OpenDB(path,sql))
		{
			return true;
		}
		return false;
	}

	bool InsertDataVideo(CString path,CString locNo,CString date,CString videoPath)
	{
		CString sql;
		sql.Format(L"UPDATE T_PLJS_JCBJ SET VIDEO_PATH = '%s' WHERE CAR_NAME = '%s' AND HAPPEN_DATE = to_date( '%s', 'YYYY/MM/DD hh24:mi:ss' )",videoPath,locNo,date);
		if(OpenDB(path,sql))
		{
			return true;
		}
		return false;
	}

};
