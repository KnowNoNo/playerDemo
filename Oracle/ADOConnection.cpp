// ADOConnection.cpp : 实现文件
//

#include "stdafx.h"
#include "ADOConnection.h"


// CADOConnection
///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		构造函数
///////////////////////////////////////////////////////////////////////////////
CADOConnection::CADOConnection()
{
	//this->InitAdoObject();
}

bool CADOConnection::InitAdoObject(void)
{
	if (this->m_pConnection != NULL)
	{
		return true;
	}
	else /*if (SUCCEEDED(m_pConnection.CreateInstance("ADODB.Connection")))*/
	{
		if (SUCCEEDED(m_pConnection.CreateInstance("ADODB.Connection")))
		{
			if (this->m_pConnection != NULL)
			{
				m_pConnection->ConnectionTimeout = 5;
				return true;
			}
		}
		else
		{
			//AfxMessageBox("CreateInstance ADODB.Connection is failed !");
			return false;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		析构函数
///////////////////////////////////////////////////////////////////////////////
CADOConnection::~CADOConnection()
{
	this->Close();
	this->m_pConnection = NULL;
}

// CADOConnection 成员函数

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		检查连接对象是否打开
///////////////////////////////////////////////////////////////////////////////
BOOL CADOConnection::IsOpen()
{
	try
	{
		if (m_pConnection != NULL)
		{
			return (m_pConnection->State == adStateOpen);
		}
		else
			return FALSE;
	}
	catch (_com_error e)
	{
		// CString errormessage;
		// errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage());
		return FALSE;
	} 
}

///////////////////////////////////////////////////////////////////////////////
// 参数说明 :
// LPCTSTR			lpszConnect			连接字符串, 包含连接信息.
// LONG				lOptions			可选.决定该连接数据源的方式:
//										adConnectUnspecified = -1	(默认)同步 
//										adAsyncConnect = 16			异步
// 功能描述 :		建立到数据源的连接
///////////////////////////////////////////////////////////////////////////////
BOOL CADOConnection::Open(CString  /*strConnect*/, LONG lOptions)
{
	try
	{
		if (!this->InitAdoObject())
		{
			return FALSE;
		}
		if (m_pConnection != NULL)
		{
			this->m_pConnection->ConnectionString = (_bstr_t)(this->m_strConnect);
			if (m_pConnection->Open("", "", "", lOptions) == S_OK)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
			//return (m_pConnection->Open("", "", "", lOptions) == S_OK);
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		//m_pConnection = NULL;//add by niuxiaofei 防止oracle地址输入错误，造成打开异常，但是connect指针不为空。
		this->Close();
		CString errormessage;
		errormessage.Format(L"连接数据库失败!\r\n错误信息:%s",e.ErrorMessage());
		//AfxMessageBox(errormessage);
		return FALSE;
	} 
}

///////////////////////////////////////////////////////////////////////////////
// LPCTSTR			strSQL			SQL语句, 表名, 存储过程调用或持久 Recordset 文件名.
// LONG				lOptions		可选. 长整型值, 用于指示 strSQL 参数的类型. 可为下列常量之一.
//									adCmdText			指示strSQL为命令文本, 即普通的SQL语句. 
//									adCmdTable			指示ADO生成SQL查询返回以 strSQL 命名的表中的所有行. 
//									adCmdTableDirect	指示所作的更改在strSQL中命名的表中返回所有行. 
//									adCmdStoredProc		指示strSQL为存储过程. 
//									adCmdUnknown		指示strSQL参数中的命令类型为未知. 
//									adCmdFile			指示应从在strSQL中命名的文件中恢复保留(保存的)Recordset. 
//									adAsyncExecute		指示应异步执行strSQL. 
//									adAsyncFetch		指示在提取 Initial Fetch Size 属性中指定的初始数量后, 应该异步提取所有剩余的行. 如果所需的行尚未提取, 主要的线程将被堵塞直到行重新可用. 
//									adAsyncFetchNonBlocking 指示主要线程在提取期间从未堵塞. 如果所请求的行尚未提取, 当前行自动移到文件末尾. 
// 功能描述 :		执行指定的查询、SQL 语句、存储过程等.
///////////////////////////////////////////////////////////////////////////////
BOOL CADOConnection::Execute(LPCTSTR strSQL, LONG lOptions)
{
	try
	{
		return SUCCEEDED(m_pConnection->Execute(_bstr_t(strSQL), NULL, lOptions));
	}
	catch (_com_error e)
	{
		//MessageBox(NULL, e.ErrorMessage(), "DB error", MB_OK);
		return FALSE;
	} 
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		关闭到数据源的连接
///////////////////////////////////////////////////////////////////////////////
void CADOConnection::Close()
{
	try
	{
		if (m_pConnection!=NULL)
		{
			if (m_pConnection->State==adStateOpen)
			{
				m_pConnection->Close();
				m_pConnection= NULL;
			}
		}
	}
	catch(_com_error e)
	{
		CString errormessage;
		errormessage.Format(L"CADOConnection::Close() - 错误信息 : %s",e.ErrorMessage());
		TRACE("%s\n", errormessage);
	} 
}