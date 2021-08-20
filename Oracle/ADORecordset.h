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
	LONG					GetFieldsCount();						// ��ȡ�ֶε�����	
	Fields *				GetFields();							// ��ȡ�ֶ�	
	_variant_t				GetCollect();							// ��÷���ֵ
	//======================= �������� ==============================
	BOOL					IsEOF();		// �Ƿ�Ϊ����	
	BOOL					IsBOF();		// �Ƿ�Ϊ��ʼ
	BOOL					MoveFirst();	// ��¼��ָ���ƶ�
	BOOL					MoveLast();		// ��¼��ָ���ƶ�
	BOOL					MovePrevious();	// ��¼��ָ���ƶ�
	BOOL					MoveNext();		// ��¼��ָ���ƶ�
	void					Close();		// �رռ�¼��
	BOOL					Open(LPCSTR lpszSQL, _ConnectionPtr pConnection, CursorTypeEnum cursorType = adOpenDynamic, ADODB::LockTypeEnum lockType = adLockOptimistic, LONG lOptions = adCmdUnspecified);	// �򿪼�¼��
	//======================= �������� ==============================
	// ����
	BOOL					SetValueBool(CString name, BOOL bValue);
	BOOL					SetValueBool(LONG index, BOOL bValue);
	// DWORD���ͣ����Է���Date���ͣ�
	DWORD					GetValueDWORD(LONG index);
	DWORD					GetValueDWORD(CString name);
	// ������
	BOOL					SetValueLong(CString name, LONG lValue);
	BOOL					SetValueLong(LONG index, LONG lValue);
	// �ַ���
	BOOL					SetValueString(CString name, CString strValue);
	BOOL					SetValueString(LONG index, CString strValue);
	// ������
	BOOL					SetValueByte(CString name, BYTE * pValue, LONG nLen);
	BOOL					SetValueByte(LONG index, BYTE * pValue, LONG nLen);
	//======================= ��ȡ���� ==============================
	// ����
	BOOL					GetValueBool(CString name);
	BOOL					GetValueBool(LONG index);
	// ������
	LONG					GetValueLong(CString name);
	LONG					GetValueLong(LONG index);
	// �ַ���
	CString					GetValueString(CString name);
	CString					GetValueString(LONG index);
	// ʱ��
	COleDateTime            GetValueDate(CString name);  
	// ������
	DB_BYTE_ARRAY *			GetValueByte(CString name);
	DB_BYTE_ARRAY *			GetValueByte(LONG index);
	BOOL					AddNew(void);
	BOOL					Update(void);
	LONG					GetRecordCount(void);
};