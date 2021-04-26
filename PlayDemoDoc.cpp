
// PlayDemoDoc.cpp : CPlayDemoDoc 类的实现
//

#include "stdafx.h"
#include "PlayDemo.h"

#include "PlayDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Csdi4Doc

IMPLEMENT_DYNCREATE(CPlayDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlayDemoDoc, CDocument)
END_MESSAGE_MAP()


// Csdi4Doc 构造/析构

CPlayDemoDoc::CPlayDemoDoc()
{
	// TODO: 在此添加一次性构造代码

}

CPlayDemoDoc::~CPlayDemoDoc()
{
}

BOOL CPlayDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Csdi4Doc 序列化

void CPlayDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CPlayDemoDoc 诊断

#ifdef _DEBUG
void CPlayDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPlayDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Csdi4Doc 命令
