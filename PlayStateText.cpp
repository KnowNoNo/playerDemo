// PlayStateText.cpp: implementation of the CPlayStateText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlayDemo.h"
#include "PlayStateText.h"
#include "CharactorTansfer.h"

#include <string>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayStateText::CPlayStateText()
{
	Clear();
	m_pFont = NULL;
}

CPlayStateText::~CPlayStateText()
{
	delete m_pFont;
}

void CPlayStateText::SetState(CString szState)
{
	if(szState == L"")
		return;
	m_szState = szState;
}

void CPlayStateText::SetPlayedTime(int nPlayedTime , int nTotalTime)
{
	m_nTotalTime = nTotalTime;
	m_nPlayedTime = nPlayedTime;
}

void CPlayStateText::SetPlayedFrame(int nPlayedFrame, int nTotoalFrame)
{
	m_nTotalFrame = nTotoalFrame;
	m_nPlayedFrame = nPlayedFrame;
}

void CPlayStateText::SetBitrate(double dbBitrate)
{
	m_dbBitrate = dbBitrate;
}

void CPlayStateText::SetRate(int nRate)
{
	m_nRate = nRate;
}

void CPlayStateText::SetPictureSize(int nWidth, int nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

void CPlayStateText::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	m_pFont = new CFont;
	m_pFont->CreatePointFont(1,_T("微软雅黑"));
	/*font.CreateFont(12,
		0,0,0,
		FW_NORMAL,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("宋体"));*/
	CWnd::FromHandle(m_hWnd)->SetFont(m_pFont, TRUE);
}

void CPlayStateText::Clear()
{
	m_szState = CString("");
	m_nPlayedTime = 0;
	m_nTotalTime = 0;
	m_nPlayedFrame = 0;
	m_nTotalFrame = 0;
	m_dbBitrate = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nRate = 0;
}

void ChangeChar(char* szBuf, int nSize, char cSrc, char cDest)
{
	for(int i = 0; i < nSize; i++)
	{
		if(szBuf[i] == cSrc) szBuf[i] = cDest;
	}
}

void CPlayStateText::Show()
{
	if(!m_nTotalFrame || !m_nTotalTime)
	{
		int nHour = (m_nPlayedTime/3600) % 24;
		int nMinute = (m_nPlayedTime%3600) /60;
		int nSecond = (m_nPlayedTime%60);
		CString szText;
		szText.Format(L"%-85s%4d*%4d%8df/s%8dkb/s", m_szState.GetBuffer(m_szState.GetLength()), m_nWidth, m_nHeight, m_nRate, (int)m_dbBitrate);
		SetWindowText(m_hWnd,szText);
	}
	else
	{
		int nHour = (m_nPlayedTime/3600) % 24;
		int nMinute = (m_nPlayedTime%3600) /60;
		int nSecond = (m_nPlayedTime%60);
		int nHour2 = (m_nTotalTime/3600) % 24;
		int nMinute2 = (m_nTotalTime%3600) /60;
		int nSecond2= (m_nTotalTime%60);

		CString szText;
		//swprintf(szText, L"%-40s%4d*%4d%4df/s%8dkb/s%6d/%-7d%02d:%02d:%02d/%02d:%02d:%02d", 
		//	m_szState.GetBuffer(m_szState.GetLength()), m_nWidth, m_nHeight, m_nRate, (int)m_dbBitrate, m_nPlayedFrame, m_nTotalFrame,
		//	nHour, nMinute, nSecond, nHour2, nMinute2, nSecond2);
		szText.Format(L"%-25s分辨率:%4d*%4d  帧数:%6d/%-7d    时间:%02d:%02d:%02d/%02d:%02d:%02d", 
			m_szState.GetBuffer(m_szState.GetLength()), m_nWidth, m_nHeight,m_nPlayedFrame, m_nTotalFrame,
			nHour, nMinute, nSecond, nHour2, nMinute2, nSecond2);

		SetWindowText(m_hWnd,szText);
	}
}