// ADORecordset.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ADORecordset.h"
#include ".\adorecordset.h"

///////////////////////////////////////////////////////////////////////////////
// �������� :		���캯��
///////////////////////////////////////////////////////////////////////////////
CADORecordset::CADORecordset()
{
}

///////////////////////////////////////////////////////////////////////////////
// �������� :		��������
///////////////////////////////////////////////////////////////////////////////
CADORecordset::~CADORecordset()
{
	Close();
}

///////////////////////////////////////////////////////////////////////////////
// ����˵�� :
// LPCSTR			lpszSQL			SQL���
// _ConnectionPtr	pConnection		
// CursorTypeEnum	cursorType		�α�����
//									adOpenForwardOnly  �򿪽���ǰ�����α�. 
//									adOpenKeyset       �򿪼��������α�. 
//									adOpenDynamic      �򿪶�̬�����α�. 
//									adOpenStatic       �򿪾�̬�����α�. 
// LockTypeEnum		lockType		��������(����)
//									adLockReadOnly         ֻ�� - ���ܸı�����. 
//									adLockPessimistic      ����ʽ���� - ͨ��ͨ���ڱ༭ʱ������������Դ�ļ�¼. 
//									adLockOptimistic       ����ʽ���� - ֻ�ڵ��� Update ����ʱ��������¼. 
//									adLockBatchOptimistic  ����ʽ������ - ����������ģʽ(����������ģʽ���). 
// LONG				lOptionsstrSQL	����������
//									adCmdText        ָʾstrSQLΪ�����ı�, ����ͨ��SQL���. 
//									adCmdTable       ָʾADO����SQL��ѯ������ strSQL �����ı��е�������. 
//									adCmdTableDirect ָʾ�����ĸ�����strSQL�������ı��з���������. 
//									adCmdStoredProc  ָʾstrSQLΪ�洢����. 
//									adCmdUnknown     ָʾstrSQL�����е���������Ϊδ֪.
// �������� :		�򿪼�¼��
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
// �������� :		�رռ�¼��
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
// �������� :		��¼��ָ���ƶ�
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
		TRACE("Warning: MoveFirst ���������쳣.");
		return FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// �������� :		��¼��ָ���ƶ�
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
		TRACE("Warning: MoveLast ���������쳣.");
		return FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// �������� :		��¼��ָ���ƶ�
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
		TRACE("Warning: MovePrevious ���������쳣.");
		return FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// �������� :		��¼��ָ���ƶ�
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
		TRACE("Warning: MoveNext ���������쳣.");
		return FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// �������� :		�Ƿ�Ϊ��ʼ
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
// �������� :		�Ƿ�Ϊ����
///////////////////////////////////////////////////////////////////////////////
BOOL CADORecordset::IsEOF()
{
	try
	{
		return (m_pRecordset->adoEOF) ? TRUE : FALSE;
	}
	catch(_com_error e)
	{
		//TRACE("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage());
		return TRUE;
	} 
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// �������� :		ȡ�ø����ֶ����Ƶĳ����ε�ֵ
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
// �������� :		��ȡ�ֶε�����
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
// �������� :		��ȡ����ֵ	
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
// �������� :		��������
///////////////////////////////////////////////////////////////////////////////
// ����
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
		TRACE("[����] SetValueBool(CString name, BOOL bValue) ����\n");
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
		TRACE("[����] SetValueBool(LONG index, BOOL bValue) ����\n");
		return FALSE;
	}	
}
// ������
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
		TRACE("[����] SetValueLong(CString name, LONG lValue) ����\n");
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
		TRACE("[����] SetValueLong(LONG index, LONG lValue) ����\n");
		return FALSE;
	}
}
// �ַ���
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
		TRACE("[����] SetValueString(CString name, CString strValue) ����\n");
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
		TRACE("[����] SetValueString(LONG index, CString strValue) ����\n");
		return FALSE;
	}
}
// ������
BOOL CADORecordset::SetValueByte(CString name, BYTE * pValue, LONG nLen)
{
	return TRUE;
}
BOOL CADORecordset::SetValueByte(LONG index, BYTE * pValue, LONG nLen)
{
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// �������� :		��ȡ����
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
			case VT_BOOL:			//������
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
			case VT_BOOL:			//������
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
			case VT_I2:				//������
				dwValue = (DWORD)var.iVal;
				break;
			case VT_UI2:			//�޷��Ŷ�����
				dwValue = (DWORD)var.uiVal;
				break;
			case VT_VOID:			//
			case VT_INT:			//����
			case VT_I4:				//����
				dwValue = (DWORD)var.intVal;
				break;
			case VT_I8:				//������
				dwValue = (DWORD)var.lVal;
				break;
			case VT_UINT:			//�޷�������
			case VT_UI4:			//�޷�������
				dwValue = (DWORD)var.uintVal;
				break;
			case VT_UI8:			//�޷��ų�����
				dwValue = (DWORD)var.ulVal;
				break;
			case VT_R8:				//10g���ݿ��ѯ�������ͣ���Ҫ��var.dblval
				dwValue = (LONG)var.dblVal;
				break;
			case VT_DECIMAL:
				dwValue = (DWORD)var.decVal.Lo32;
				break;
			case VT_DATE: //������
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
				TRACE("����: δ��������� GetValueLong(LONG index)\n");
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
			case VT_I2:				//������
				dwValue = (DWORD)var.iVal;
				break;
			case VT_UI2:			//�޷��Ŷ�����
				dwValue = (DWORD)var.uiVal;
				break;
			case VT_VOID:			//
			case VT_INT:			//����
			case VT_I4:				//����
				dwValue = (DWORD)var.intVal;
				break;
			case VT_I8:				//������
				dwValue = (DWORD)var.lVal;
				break;
			case VT_UINT:			//�޷�������
			case VT_UI4:			//�޷�������
				dwValue = (DWORD)var.uintVal;
				break;
			case VT_UI8:			//�޷��ų�����
				dwValue = (DWORD)var.ulVal;
				break;
			case VT_R8:				//10g���ݿ��ѯ�������ͣ���Ҫ��var.dblval
				dwValue = (LONG)var.dblVal;
				break;
			case VT_DECIMAL:
				dwValue = (DWORD)var.decVal.Lo32;
				break;
			case VT_DATE: //������
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
				TRACE("����: δ��������� GetValueLong(CString name)\n");
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
			case VT_I2:				//������
				lValue = (LONG)var.iVal;
				break;
			case VT_UI2:			//�޷��Ŷ�����
				lValue = (LONG)var.uiVal;
				break;
			case VT_VOID:			//
			case VT_INT:			//����
			case VT_I4:				//����
				lValue = (LONG)var.intVal;
				break;
			case VT_I8:				//������
				lValue = (LONG)var.lVal;
				break;
			case VT_UINT:			//�޷�������
			case VT_UI4:			//�޷�������
				lValue = (LONG)var.uintVal;
				break;
			case VT_R8:				//10g���ݿ��ѯ�������ͣ���Ҫ��var.dblval
				lValue = (LONG)var.dblVal;
				break;
			case VT_UI8:			//�޷��ų�����
				lValue = (LONG)var.ulVal;
				break;
			case VT_DECIMAL:
				lValue = (LONG)var.decVal.Lo32;
				break;
			default:
				TRACE("����: δ��������� GetValueLong(CString name)\n");
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
			case VT_I2:				//������
				lValue = (LONG)var.iVal;
				break;
			case VT_UI2:			//�޷��Ŷ�����
				lValue = (LONG)var.uiVal;
				break;
			case VT_VOID:			//
			case VT_INT:			//����
			case VT_I4:				//����
				lValue = (LONG)var.intVal;
				break;
			case VT_I8:				//������
				lValue = (LONG)var.lVal;
				break;
			case VT_UINT:			//�޷�������
			case VT_UI4:			//�޷�������
				lValue = (LONG)var.uintVal;
				break;
			case VT_R8:				//10g���ݿ��ѯ�������ͣ���Ҫ��var.dblval
				lValue = (LONG)var.dblVal;
				break;
			case VT_UI8:			//�޷��ų�����
				lValue = (LONG)var.ulVal;
				break;
			case VT_DECIMAL:
				lValue = (LONG)var.decVal.Lo32;
				break;
			default:
				TRACE("����: δ��������� GetValueLong(LONG index)\n");
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
		case VT_DATE:    //������
			da.SetStatus(COleDateTime::DateTimeStatus(0));
			dt = var.date;
			da = COleDateTime(dt); 
			strValue = da.Format(L"%H:%M");
			break;
		default:
			TRACE(L"����: δ��������� GetTimeString, ֻ����DATE����\n");
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
			case VT_BSTR:    //�ַ���
			case VT_LPSTR:   //�ַ���
			case VT_LPWSTR:  //�ַ���
				strValue = (LPCTSTR)(_bstr_t)var;
				break;
			case VT_DATE:    //������
				dt = var.date;
				da = COleDateTime(dt); 
				strValue = da.Format(L"%Y-%m-%d %H:%M:%S");
				break;
			case VT_DECIMAL:
				strValue = (LPCTSTR)(_bstr_t)var;
				break;
			default:
				TRACE(L"����: δ��������� GetValueString(CString name)\n");
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
			case VT_BSTR://�ַ���
			case VT_LPSTR://�ַ���
			case VT_LPWSTR://�ַ���
				strValue = (LPCTSTR)(_bstr_t)var;
				break;
			case VT_DATE: //������
				dt = var.date;
				da = COleDateTime(dt); 
				strValue = da.Format(L"%Y-%m-%d %H:%M:%S");
				break;
			case VT_DECIMAL:
				strValue = (LPCTSTR)(_bstr_t)var;
			default:
				TRACE(L"����: δ��������� GetValueString(LONG index)\n");
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
				case 0x2011:	// ACCESS OLE����
				case VT_BLOB:
					{
						pArray = new DB_BYTE_ARRAY();
						BYTE * p = NULL;
						SafeArrayAccessData(var.parray,(void  **)&p);    //�õ�ָ�����ݵ�ָ��  
						pArray->pBuffer = new BYTE[nSize];
						memcpy(pArray->pBuffer, p, nSize);
						pArray->nLen = nSize;
						SafeArrayUnaccessData(var.parray);  
					}
					break;
				default:
					TRACE("����: δ��������� GetValueByte(CString name)\n");
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
				case 0x2011:	// ACCESS OLE����
				case VT_BLOB:
					{
						pArray = new DB_BYTE_ARRAY();
						BYTE * p = NULL;
						SafeArrayAccessData(var.parray,(void  **)&p);    //�õ�ָ�����ݵ�ָ��
						pArray->pBuffer = new BYTE[nSize];
						memcpy(pArray->pBuffer, p, nSize);
						pArray->nLen = nSize;
						SafeArrayUnaccessData(var.parray);  
					}
					break;
				default:
					TRACE("����: δ��������� GetValueByte(LONG index)\n");
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
		TRACE("AddNew��������: %s\n", e.ErrorMessage());
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
		TRACE("Update��������: %s\n", e.ErrorMessage());
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
		TRACE("GetRecordCount��������: %s\n", e.ErrorMessage());
		return 0;
	} 
}
