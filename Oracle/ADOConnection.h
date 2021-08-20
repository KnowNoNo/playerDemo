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
	//初始化ADO连接对象
	bool InitAdoObject(void);

public:

	// 检查连接对象是否打开
	BOOL				IsOpen();
	// 建立到数据源的连接
	BOOL				Open(CString strConnect, LONG lOptions = adConnectUnspecified);
	// 执行指定的查询、SQL 语句、存储过程等.
	BOOL				Execute(LPCTSTR strSQL, LONG lOptions = adCmdText);
	// 关闭到数据源的连接
	void				Close();
};