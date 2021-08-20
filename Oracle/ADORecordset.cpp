// ADORecordset.cpp : 实现文件
//

#include "stdafx.h"
#include "ADORecordset.h"
#include ".\adorecordset.h"

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		构造函数
///////////////////////////////////////////////////////////////////////////////
CADORecordset::CADORecordset()
{
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		析构函数
///////////////////////////////////////////////////////////////////////////////
CADORecordset::~CADORecordset()
{
	Close();
}

///////////////////////////////////////////////////////////////////////////////
// 参数说明 :
// LPCSTR			lpszSQL			SQL语句
// _ConnectionPtr	pConnection		
// CursorTypeEnum	cursorType		游标类型
//									adOpenForwardOnly  打开仅向前类型游标. 
//									adOpenKeyset       打开键集类型游标. 
//									adOpenDynamic      打开动态类型游标. 
//									adOpenStatic       打开静态类型游标. 
// LockTypeEnum		lockType		锁定类型(并发)
//									adLockReadOnly         只读 - 不能改变数据. 
//									adLockPessimistic      保守式锁定 - 通常通过在编辑时立即锁定数据源的记录. 
//									adLockOptimistic       开放式锁定 - 只在调用 Update 方法时才锁定记录. 
//									adLockBatchOptimistic  开放式批更新 - 用于批更新模式(与立即更新模式相对). 
// LONG				lOptionsstrSQL	参数的类型
//									adCmdText        指示strSQL为命令文本, 即普通的SQL语句. 
//									adCmdTable       指示ADO生成SQL查询返回以 strSQL 命名的表中的所有行. 
//									adCmdTableDirect 指示所作的更改在strSQL中命名的表中返回所有行. 
//									adCmdStoredProc  指示strSQL为存储过程. 
//									adCmdUnknown     指示strSQL参数中的命令类型为未知.
// 功能描述 :		打开记录集
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::Open(LPCSTR lpszSQL, _ConnectionPtr	pConnection, CursorTypeEnum cursorType, ADODB::LockTypeEnum lockType, LONG lOptions)
{
	if (!SUCCEEDED(m_pRecordset.CreateInstance("ADODB.Recordset")))
	{
		return FALSE;
	}
	try
	{
		m_strSQL = lpszSQL;
		if (m_strSQL.IsEmpty())
			return FALSE;

		if (pConnection == NULL)
			return FALSE;

		m_pRecordset->Open(_bstr_t(m_strSQL), pConnection.GetInterfacePtr(), cursorType, lockType, lOptions);
		if (m_pRecordset == NULL) 
			return FALSE;
		return TRUE;
	}
	catch (_com_error e)
	{
		//MessageBox(NULL, e.ErrorMessage(), "DB error", MB_OK);
		TRACE("--%s\r\n",e.ErrorMessage());
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		关闭记录集
///////////////////////////////////////////////////////////////////////////////
void CADORecordset::Close()
{
	try
	{
		if (m_pRecordset != NULL && m_pRecordset->State != adStateClosed)
		{
			m_pRecordset->Close();
		}
	}
	catch(_com_error e)
	{
	} 
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		记录集指针移动
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::MoveFirst()
{
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveFirst());
		}
	}
	catch (_com_error e)
	{
		TRACE("Warning: MoveFirst 方法发生异常.");
		return FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		记录集指针移动
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::MoveLast()
{
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveLast());
		}
	}
	catch (_com_error e)
	{
		TRACE("Warning: MoveLast 方法发生异常.");
		return FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		记录集指针移动
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::MovePrevious()
{
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MovePrevious());
		}
	}
	catch (_com_error e)
	{
		TRACE("Warning: MovePrevious 方法发生异常.");
		return FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		记录集指针移动
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::MoveNext()
{
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveNext());
		}
	}
	catch (_com_error e)
	{
		TRACE("Warning: MoveNext 方法发生异常.");
		return FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		是否为开始
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::IsBOF()
{
	try
	{
		return (m_pRecordset->adoBOF) ? TRUE : FALSE;
	}
	catch(_com_error e)
	{
		return TRUE;
	} 
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		是否为结束
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::IsEOF()
{
	try
	{
		return (m_pRecordset->adoEOF) ? TRUE : FALSE;
	}
	catch(_com_error e)
	{
		//TRACE("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage());
		return TRUE;
	} 
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		取得给定字段名称的长整形的值
///////////////////////////////////////////////////////////////////////////////
Fields * CADORecordset::GetFields()
{
	FieldsPtr pFields;
	try
	{
		if (SUCCEEDED(m_pRecordset->get_Fields(&pFields)))
		{
			return pFields;
		}
	}
	catch (_com_error e)
	{
		return NULL;
	} 
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		获取字段的数量
///////////////////////////////////////////////////////////////////////////////
LONG CADORecordset::GetFieldsCount()
{
	try
	{
		return GetFields()->Count;
	}
	catch(_com_error e)
	{
		return 0;
	} 
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		获取返回值	
// --zlh 2021-06
///////////////////////////////////////////////////////////////////////////////
_variant_t CADORecordset::GetCollect()
{
	try
	{
		return m_pRecordset->GetCollect(0L);
	}
	catch(_com_error e)
	{
		return 0;
	} 
}

///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		设置数据
///////////////////////////////////////////////////////////////////////////////
// 布尔
BOOL CADORecordset::SetValueBool(CString name, BOOL bValue)
{
	try
	{
		if (m_pRecordset != NULL)
		{
			m_pRecordset->PutCollect(_variant_t(name), _variant_t(bValue));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		TRACE("[警告] SetValueBool(CString name, BOOL bValue) 错误\n");
		return FALSE;
	}	
}
BOOL CADORecordset::SetValueBool(LONG index, BOOL bValue)
{
	try
	{
		if (m_pRecordset != NULL)
		{
			m_pRecordset->PutCollect(_variant_t(index), _variant_t(bValue));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		TRACE("[警告] SetValueBool(LONG index, BOOL bValue) 错误\n");
		return FALSE;
	}	
}
// 长整型
BOOL CADORecordset::SetValueLong(CString name, LONG lValue)
{
	try
	{
		if (m_pRecordset != NULL)
		{
			m_pRecordset->PutCollect(_variant_t(name), _variant_t(lValue));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		TRACE("[警告] SetValueLong(CString name, LONG lValue) 错误\n");
		return FALSE;
	}
}
BOOL CADORecordset::SetValueLong(LONG index, LONG lValue)
{
	try
	{
		if (m_pRecordset != NULL)
		{
			m_pRecordset->PutCollect(_variant_t(index), _variant_t(lValue));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		TRACE("[警告] SetValueLong(LONG index, LONG lValue) 错误\n");
		return FALSE;
	}
}
// 字符串
BOOL CADORecordset::SetValueString(CString name, CString strValue)
{
	if (strValue.Trim()=="")
		return FALSE;
	try
	{
		if (m_pRecordset != NULL)
		{
			m_pRecordset->PutCollect(_variant_t(name), _variant_t(strValue));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		TRACE("[警告] SetValueString(CString name, CString strValue) 错误\n");
		return FALSE;
	}
}
BOOL CADORecordset::SetValueString(LONG index, CString strValue)
{
	if (strValue.Trim()=="")
		return FALSE;
	try
	{
		if (m_pRecordset != NULL)
		{
			m_pRecordset->PutCollect(_variant_t(index), _variant_t(strValue));
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	catch (_com_error e)
	{
		TRACE("[警告] SetValueString(LONG index, CString strValue) 错误\n");
		return FALSE;
	}
}
// 二进制
BOOL CADORecordset::SetValueByte(CString name, BYTE * pValue, LONG nLen)
{
	return TRUE;
}
BOOL CADORecordset::SetValueByte(LONG index, BYTE * pValue, LONG nLen)
{
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// 功能描述 :		读取数据
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::GetValueBool(CString name)
{
	BOOL bValue = FALSE;
	_variant_t var;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return bValue;
		if (m_pRecordset->adoEOF)
			return bValue;
		var = m_pRecordset->GetCollect(_variant_t(name));
		switch (var.vt)
		{
			case VT_BOOL:			//布尔型
				bValue = var.boolVal ? TRUE : FALSE;
				break;
		}
		return bValue;
	}
	catch (_com_error e)
	{
		return bValue;
	}	
}

BOOL CADORecordset::GetValueBool(LONG index)
{
	BOOL bValue = FALSE;
	_variant_t var;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return bValue;
		if (m_pRecordset->adoEOF)
			return bValue;
		var = m_pRecordset->GetCollect(_variant_t(index));
		switch (var.vt)
		{
			case VT_BOOL:			//布尔型
				bValue = var.boolVal ? TRUE : FALSE;
				break;
		}
		return bValue;
	}
	catch (_com_error e)
	{
		return bValue;
	}	
}

DWORD CADORecordset::GetValueDWORD(LONG index)
{
	DWORD dwValue = 0;
	_variant_t var;
	DATE dt;
	COleDateTime da;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return dwValue;
		if (m_pRecordset->adoEOF)
			return dwValue;
		var = m_pRecordset->GetCollect(_variant_t(index));
		switch (var.vt)
		{
			case VT_I2:				//短整型
				dwValue = (DWORD)var.iVal;
				break;
			case VT_UI2:			//无符号短整型
				dwValue = (DWORD)var.uiVal;
				break;
			case VT_VOID:			//
			case VT_INT:			//整型
			case VT_I4:				//整型
				dwValue = (DWORD)var.intVal;
				break;
			case VT_I8:				//长整型
				dwValue = (DWORD)var.lVal;
				break;
			case VT_UINT:			//无符号整型
			case VT_UI4:			//无符号整型
				dwValue = (DWORD)var.uintVal;
				break;
			case VT_UI8:			//无符号长整型
				dwValue = (DWORD)var.ulVal;
				break;
			case VT_R8:				//10g数据库查询序列类型，需要用var.dblval
				dwValue = (LONG)var.dblVal;
				break;
			case VT_DECIMAL:
				dwValue = (DWORD)var.decVal.Lo32;
				break;
			case VT_DATE: //日期型
				{
					dt = var.date;
					da = COleDateTime(dt);
					int iYear = da.GetYear();
					int iMonth = da.GetMonth();
					int iDay = da.GetDay();
					int iHour = da.GetHour();
					int iSecond = da.GetSecond();
					int iMinute =da.GetMinute();
					CTime ct(iYear, iMonth, iDay, iHour, iMinute, iSecond);
					dwValue = (DWORD)ct.GetTime();
				}
				break;
			default:
				TRACE("警告: 未处理的类型 GetValueLong(LONG index)\n");
		}
		return dwValue;
	}
	catch (_com_error e)
	{
		return dwValue;
	}
}

DWORD CADORecordset::GetValueDWORD(CString name)
{
	DWORD dwValue = 0;
	_variant_t var;
	DATE dt;
	COleDateTime da;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return dwValue;
		if (m_pRecordset->adoEOF)
			return dwValue;
		var = m_pRecordset->GetCollect(_variant_t(name));
		switch (var.vt)
		{
			case VT_I2:				//短整型
				dwValue = (DWORD)var.iVal;
				break;
			case VT_UI2:			//无符号短整型
				dwValue = (DWORD)var.uiVal;
				break;
			case VT_VOID:			//
			case VT_INT:			//整型
			case VT_I4:				//整型
				dwValue = (DWORD)var.intVal;
				break;
			case VT_I8:				//长整型
				dwValue = (DWORD)var.lVal;
				break;
			case VT_UINT:			//无符号整型
			case VT_UI4:			//无符号整型
				dwValue = (DWORD)var.uintVal;
				break;
			case VT_UI8:			//无符号长整型
				dwValue = (DWORD)var.ulVal;
				break;
			case VT_R8:				//10g数据库查询序列类型，需要用var.dblval
				dwValue = (LONG)var.dblVal;
				break;
			case VT_DECIMAL:
				dwValue = (DWORD)var.decVal.Lo32;
				break;
			case VT_DATE: //日期型
				{
					dt = var.date;
					da = COleDateTime(dt);
					int iYear = da.GetYear();
					int iMonth = da.GetMonth();
					int iDay = da.GetDay();
					int iHour = da.GetHour();
					int iSecond = da.GetSecond();
					int iMinute =da.GetMinute();
					CTime ct(iYear, iMonth, iDay, iHour, iMinute, iSecond);
					dwValue = (DWORD)ct.GetTime();
					break;
				}
			default:
				TRACE("警告: 未处理的类型 GetValueLong(CString name)\n");
		}
		return dwValue;
	}
	catch (_com_error e)
	{
		return dwValue;
	}
}

LONG CADORecordset::GetValueLong(CString name)
{
	LONG lValue = 0;
	_variant_t var;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return lValue;
		if (m_pRecordset->adoEOF)
			return lValue;
		var = m_pRecordset->GetCollect(_variant_t(name));
		switch (var.vt)
		{
			case VT_I2:				//短整型
				lValue = (LONG)var.iVal;
				break;
			case VT_UI2:			//无符号短整型
				lValue = (LONG)var.uiVal;
				break;
			case VT_VOID:			//
			case VT_INT:			//整型
			case VT_I4:				//整型
				lValue = (LONG)var.intVal;
				break;
			case VT_I8:				//长整型
				lValue = (LONG)var.lVal;
				break;
			case VT_UINT:			//无符号整型
			case VT_UI4:			//无符号整型
				lValue = (LONG)var.uintVal;
				break;
			case VT_R8:				//10g数据库查询序列类型，需要用var.dblval
				lValue = (LONG)var.dblVal;
				break;
			case VT_UI8:			//无符号长整型
				lValue = (LONG)var.ulVal;
				break;
			case VT_DECIMAL:
				lValue = (LONG)var.decVal.Lo32;
				break;
			default:
				TRACE("警告: 未处理的类型 GetValueLong(CString name)\n");
		}
		return lValue;
	}
	catch (_com_error e)
	{
		return lValue;
	}
}

LONG CADORecordset::GetValueLong(LONG index)
{
	LONG lValue = 0;
	_variant_t var;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return lValue;
		if (m_pRecordset->adoEOF)
			return lValue;
		var = m_pRecordset->GetCollect(_variant_t(index));
		switch (var.vt)
		{
			case VT_I2:				//短整型
				lValue = (LONG)var.iVal;
				break;
			case VT_UI2:			//无符号短整型
				lValue = (LONG)var.uiVal;
				break;
			case VT_VOID:			//
			case VT_INT:			//整型
			case VT_I4:				//整型
				lValue = (LONG)var.intVal;
				break;
			case VT_I8:				//长整型
				lValue = (LONG)var.lVal;
				break;
			case VT_UINT:			//无符号整型
			case VT_UI4:			//无符号整型
				lValue = (LONG)var.uintVal;
				break;
			case VT_R8:				//10g数据库查询序列类型，需要用var.dblval
				lValue = (LONG)var.dblVal;
				break;
			case VT_UI8:			//无符号长整型
				lValue = (LONG)var.ulVal;
				break;
			case VT_DECIMAL:
				lValue = (LONG)var.decVal.Lo32;
				break;
			default:
				TRACE("警告: 未处理的类型 GetValueLong(LONG index)\n");
		}
		return lValue;
	}
	catch (_com_error e)
	{
		return lValue;
	}
}

COleDateTime CADORecordset::GetValueDate(CString name)
{
	CString strValue = "";
	_variant_t var;
	DATE dt;
	COleDateTime da;
	da.SetStatus(COleDateTime::DateTimeStatus(-1));
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return da;
		if (m_pRecordset->adoEOF)
			return da;
		var = m_pRecordset->GetCollect(_variant_t(name));
		switch (var.vt)
		{
		case VT_DATE:    //日期型
			da.SetStatus(COleDateTime::DateTimeStatus(0));
			dt = var.date;
			da = COleDateTime(dt); 
			strValue = da.Format(L"%H:%M");
			break;
		default:
			TRACE(L"警告: 未处理的类型 GetTimeString, 只接受DATE类型\n");
		}
		return da;
	}
	catch (_com_error e)
	{
		return da;
	}
}
CString CADORecordset::GetValueString(CString name)
{
	CString strValue = "";
	_variant_t var;
	DATE dt;
	COleDateTime da;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return strValue;
		if (m_pRecordset->adoEOF)
			return strValue;
		var = m_pRecordset->GetCollect(_variant_t(name));
		switch (var.vt)
		{
			case VT_BSTR:    //字符串
			case VT_LPSTR:   //字符串
			case VT_LPWSTR:  //字符串
				strValue = (LPCTSTR)(_bstr_t)var;
				break;
			case VT_DATE:    //日期型
				dt = var.date;
				da = COleDateTime(dt); 
				strValue = da.Format(L"%Y-%m-%d %H:%M:%S");
				break;
			case VT_DECIMAL:
				strValue = (LPCTSTR)(_bstr_t)var;
				break;
			default:
				TRACE(L"警告: 未处理的类型 GetValueString(CString name)\n");
		}
		return strValue;
	}
	catch (_com_error e)
	{
		return strValue;
	}
}
CString CADORecordset::GetValueString(LONG index)
{
	CString strValue = "";
	_variant_t var;
	DATE dt;
	COleDateTime da;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return strValue;
		if (m_pRecordset->adoEOF)
			return strValue;
		var = m_pRecordset->GetCollect(_variant_t(index));
		switch (var.vt)
		{
			case VT_BSTR://字符串
			case VT_LPSTR://字符串
			case VT_LPWSTR://字符串
				strValue = (LPCTSTR)(_bstr_t)var;
				break;
			case VT_DATE: //日期型
				dt = var.date;
				da = COleDateTime(dt); 
				strValue = da.Format(L"%Y-%m-%d %H:%M:%S");
				break;
			case VT_DECIMAL:
				strValue = (LPCTSTR)(_bstr_t)var;
			default:
				TRACE(L"警告: 未处理的类型 GetValueString(LONG index)\n");
		}
		return strValue;
	}
	catch (_com_error e)
	{
		return strValue;
	}
}

DB_BYTE_ARRAY * CADORecordset::GetValueByte(CString name)
{
	DB_BYTE_ARRAY * pArray = NULL;
	_variant_t var;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return pArray;
		if (m_pRecordset->adoEOF)
			return pArray;
		int nSize = m_pRecordset->GetFields()->GetItem(_variant_t(name))->ActualSize;
		if (nSize>0)
		{
			var = m_pRecordset->GetFields()->GetItem(_variant_t(name))->GetChunk(nSize);
			switch (var.vt)
			{
				case VT_ARRAY:
				case VT_UI1:
				case 0x2011:	// ACCESS OLE对象
				case VT_BLOB:
					{
						pArray = new DB_BYTE_ARRAY();
						BYTE * p = NULL;
						SafeArrayAccessData(var.parray,(void  **)&p);    //得到指向数据的指针  
						pArray->pBuffer = new BYTE[nSize];
						memcpy(pArray->pBuffer, p, nSize);
						pArray->nLen = nSize;
						SafeArrayUnaccessData(var.parray);  
					}
					break;
				default:
					TRACE("警告: 未处理的类型 GetValueByte(CString name)\n");
			}
		}
		return pArray;
	}
	catch (_com_error e)
	{
		return pArray;
	}
}

DB_BYTE_ARRAY * CADORecordset::GetValueByte(LONG index)
{
	DB_BYTE_ARRAY * pArray = NULL;
	_variant_t var;
	try
	{
		if (m_pRecordset == NULL || m_pRecordset->State == adStateClosed)
			return pArray;
		if (m_pRecordset->adoEOF)
			return pArray;
		int nSize = m_pRecordset->GetFields()->GetItem(_variant_t(index))->ActualSize;
		if (nSize>0)
		{
			var = m_pRecordset->GetFields()->GetItem(_variant_t(index))->GetChunk(nSize);
			switch (var.vt)
			{
				case VT_ARRAY:
				case VT_UI1:
				case 0x2011:	// ACCESS OLE对象
				case VT_BLOB:
					{
						pArray = new DB_BYTE_ARRAY();
						BYTE * p = NULL;
						SafeArrayAccessData(var.parray,(void  **)&p);    //得到指向数据的指针
						pArray->pBuffer = new BYTE[nSize];
						memcpy(pArray->pBuffer, p, nSize);
						pArray->nLen = nSize;
						SafeArrayUnaccessData(var.parray);  
					}
					break;
				default:
					TRACE("警告: 未处理的类型 GetValueByte(LONG index)\n");
			}
		}
		return pArray;
	}
	catch (_com_error e)
	{
		return pArray;
	}
}

BOOL CADORecordset::AddNew(void)
{
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->AddNew());
		}
		return FALSE;
	}
	catch (_com_error e)
	{
		TRACE("AddNew发生错误: %s\n", e.ErrorMessage());
		return FALSE;
	} 
}

BOOL CADORecordset::Update(void)
{
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->Update());
		}
		return FALSE;
	}
	catch (_com_error e)
	{
		TRACE("Update发生错误: %s\n", e.ErrorMessage());
		return FALSE;
	} 
}

LONG CADORecordset::GetRecordCount(void)
{
	LONG lCount = 0;
	try
	{
		m_pRecordset->get_RecordCount(&lCount);
		return lCount;
	}
	catch(_com_error e)
	{
		TRACE("GetRecordCount发生错误: %s\n", e.ErrorMessage());
		return 0;
	} 
}
