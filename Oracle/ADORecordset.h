#pragma once

#include "ADOConnection.h"

struct DB_BYTE_ARRAY
{
	int			nLen;
	BYTE *		pBuffer;
	DB_BYTE_ARRAY()
	{
		nLen = 0;
		pBuffer = NULL;
	}
	~DB_BYTE_ARRAY()
	{
		nLen = 0;
		delete pBuffer;
	}
};

class CADORecordset : public CObject
{
public:
	_RecordsetPtr			m_pRecordset;
	CString					m_strSQL;
public:
	CADORecordset();
	virtual					~CADORecordset();	
	LONG					GetFieldsCount();						// 获取字段的数量	
	Fields *				GetFields();							// 获取字段	
	_variant_t				GetCollect();							// 获得返回值
	//======================= 基本操作 ==============================
	BOOL					IsEOF();		// 是否为结束	
	BOOL					IsBOF();		// 是否为开始
	BOOL					MoveFirst();	// 记录集指针移动
	BOOL					MoveLast();		// 记录集指针移动
	BOOL					MovePrevious();	// 记录集指针移动
	BOOL					MoveNext();		// 记录集指针移动
	void					Close();		// 关闭记录集
	BOOL					Open(LPCSTR lpszSQL, _ConnectionPtr pConnection, CursorTypeEnum cursorType = adOpenDynamic, ADODB::LockTypeEnum lockType = adLockOptimistic, LONG lOptions = adCmdUnspecified);	// 打开记录集
	//======================= 设置数据 ==============================
	// 布尔
	BOOL					SetValueBool(CString name, BOOL bValue);
	BOOL					SetValueBool(LONG index, BOOL bValue);
	// DWORD类型（可以返回Date类型）
	DWORD					GetValueDWORD(LONG index);
	DWORD					GetValueDWORD(CString name);
	// 长整型
	BOOL					SetValueLong(CString name, LONG lValue);
	BOOL					SetValueLong(LONG index, LONG lValue);
	// 字符串
	BOOL					SetValueString(CString name, CString strValue);
	BOOL					SetValueString(LONG index, CString strValue);
	// 二进制
	BOOL					SetValueByte(CString name, BYTE * pValue, LONG nLen);
	BOOL					SetValueByte(LONG index, BYTE * pValue, LONG nLen);
	//======================= 读取数据 ==============================
	// 布尔
	BOOL					GetValueBool(CString name);
	BOOL					GetValueBool(LONG index);
	// 长整型
	LONG					GetValueLong(CString name);
	LONG					GetValueLong(LONG index);
	// 字符串
	CString					GetValueString(CString name);
	CString					GetValueString(LONG index);
	// 时间
	COleDateTime            GetValueDate(CString name);  
	// 二进制
	DB_BYTE_ARRAY *			GetValueByte(CString name);
	DB_BYTE_ARRAY *			GetValueByte(LONG index);
	BOOL					AddNew(void);
	BOOL					Update(void);
	LONG					GetRecordCount(void);
};