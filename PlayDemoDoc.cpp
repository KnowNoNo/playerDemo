
// PlayDemoDoc.cpp : CPlayDemoDoc ���ʵ��
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


// Csdi4Doc ����/����

CPlayDemoDoc::CPlayDemoDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CPlayDemoDoc::~CPlayDemoDoc()
{
}

BOOL CPlayDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// Csdi4Doc ���л�

void CPlayDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CPlayDemoDoc ���

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


// Csdi4Doc ����
