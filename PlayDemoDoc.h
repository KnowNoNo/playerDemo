
// sdi4Doc.h : Csdi4Doc ��Ľӿ�
//


#pragma once


class CPlayDemoDoc : public CDocument
{
protected: // �������л�����
	CPlayDemoDoc();
	DECLARE_DYNCREATE(CPlayDemoDoc)

	// ����
public:

	// ����
public:

	// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	// ʵ��
public:
	virtual ~CPlayDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


