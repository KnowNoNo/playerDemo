// PlayStateText.h: interface for the CPlayStateText class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CPlayStateText  
{
public:
	CPlayStateText();
	virtual ~CPlayStateText();

public:
	void SetState(CString szState);
	void SetPlayedTime(int nPlayedTime , int nTotalTime /*0  not change*/);
	void SetPlayedFrame(int nPlayedFrame, int nTotalFrame /*0 not change*/);
	void SetBitrate(double dbBitrate);
	void SetRate(int nRate);
	void SetPictureSize(int nWidth, int nHeight);
	/*reset*/
	void Init(HWND hWnd);
	void Show();
	void Clear();
private:
	HWND	m_hWnd;

	CFont *m_pFont;
	CString m_szState;
	int m_nPlayedTime;
	int m_nTotalTime;
	int m_nPlayedFrame;
	int m_nTotalFrame;
	double m_dbBitrate;
	int m_nRate;
	int m_nWidth;
	int m_nHeight;
};
