// ADOConnection.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ADOConnection.h"


// CADOConnection
///////////////////////////////////////////////////////////////////////////////
// �������� :		���캯��
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
// �������� :		��������
///////////////////////////////////////////////////////////////////////////////
CADOConnection::~CADOConnection()
{
	this->Close();
	this->m_pConnection = NULL;
}

// CADOConnection ��Ա����

///////////////////////////////////////////////////////////////////////////////
// �������� :		������Ӷ����Ƿ��
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
		// errormessage.Format("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage());
		return FALSE;
	} 
}

///////////////////////////////////////////////////////////////////////////////
// ����˵�� :
// LPCTSTR			lpszConnect			�����ַ���, ����������Ϣ.
// LONG				lOptions			��ѡ.��������������Դ�ķ�ʽ:
//										adConnectUnspecified = -1	(Ĭ��)ͬ�� 
//										adAsyncConnect = 16			�첽
// �������� :		����������Դ������
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
		//m_pConnection = NULL;//add by niuxiaofei ��ֹoracle��ַ���������ɴ��쳣������connectָ�벻Ϊ�ա�
		this->Close();
		CString errormessage;
		errormessage.Format(L"�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage());
		//AfxMessageBox(errormessage);
		return FALSE;
	} 
}

///////////////////////////////////////////////////////////////////////////////
// LPCTSTR			strSQL			SQL���, ����, �洢���̵��û�־� Recordset �ļ���.
// LONG				lOptions		��ѡ. ������ֵ, ����ָʾ strSQL ����������. ��Ϊ���г���֮һ.
//									adCmdText			ָʾstrSQLΪ�����ı�, ����ͨ��SQL���. 
//									adCmdTable			ָʾADO����SQL��ѯ������ strSQL �����ı��е�������. 
//									adCmdTableDirect	ָʾ�����ĸ�����strSQL�������ı��з���������. 
//									adCmdStoredProc		ָʾstrSQLΪ�洢����. 
//									adCmdUnknown		ָʾstrSQL�����е���������Ϊδ֪. 
//									adCmdFile			ָʾӦ����strSQL���������ļ��лָ�����(�����)Recordset. 
//									adAsyncExecute		ָʾӦ�첽ִ��strSQL. 
//									adAsyncFetch		ָʾ����ȡ Initial Fetch Size ������ָ���ĳ�ʼ������, Ӧ���첽��ȡ����ʣ�����. ������������δ��ȡ, ��Ҫ���߳̽�������ֱ�������¿���. 
//									adAsyncFetchNonBlocking ָʾ��Ҫ�߳�����ȡ�ڼ��δ����. ��������������δ��ȡ, ��ǰ���Զ��Ƶ��ļ�ĩβ. 
// �������� :		ִ��ָ���Ĳ�ѯ��SQL ��䡢�洢���̵�.
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
// �������� :		�رյ�����Դ������
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
		errormessage.Format(L"CADOConnection::Close() - ������Ϣ : %s",e.ErrorMessage());
		TRACE("%s\n", errormessage);
	} 
}