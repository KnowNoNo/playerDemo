#pragma once

#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF") rename("BOF","adoBOF")
//no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")
//rename_namespace( "AdoNS ")   rename( "EOF ", "adoEOF ") 

#pragma warning(default:4146)
using namespace ADODB;

class CADOConnection : public CObject
{
public:
	CADOConnection();
	virtual ~CADOConnection();

public:
	_ConnectionPtr		m_pConnection;
	CString				m_strConnect;

public:
	//��ʼ��ADO���Ӷ���
	bool InitAdoObject(void);

public:

	// ������Ӷ����Ƿ��
	BOOL				IsOpen();
	// ����������Դ������
	BOOL				Open(CString strConnect, LONG lOptions = adConnectUnspecified);
	// ִ��ָ���Ĳ�ѯ��SQL ��䡢�洢���̵�.
	BOOL				Execute(LPCTSTR strSQL, LONG lOptions = adCmdText);
	// �رյ�����Դ������
	void				Close();
};