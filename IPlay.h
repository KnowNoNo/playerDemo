// IPlay.h: interface for the IPlay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPLAY_H__BEF5C3AA_222F_426B_89C2_97CAD97340D6__INCLUDED_)
#define AFX_IPLAY_H__BEF5C3AA_222F_426B_89C2_97CAD97340D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IState.h"
#include "dhplay.h"
/*
* abstrate Play Interface
*/
interface IPlay  
{
	virtual int Open(TCHAR* lpszFile, PlayParam* lpPlayParam) = 0;
	virtual int Close(PlayParam* lpPlayParam) = 0;
	
	virtual int Do(PlayParam* lpPlayParam) = 0;
	virtual TCHAR* Description(PlayParam* lpPlayParam) = 0;
	virtual int ChangeState(IPlayState* lpState) = 0;

	virtual int OpenSound(PlayParam* lpPlayParam, BOOL bOpen) = 0;
	virtual int SetAudioVolume(PlayParam* lpPlayParam, int nVolume) =0 ;
	virtual int GetAudioVolume(PlayParam* lpPlayParam) = 0;
	virtual int SetAudioWave(PlayParam* lpPlayParam, int nWave) = 0;
	
	virtual int GetProcess(PlayParam* lpPlayParam) = 0;
	virtual int Seek(PlayParam* lpPlayParam, int nPos) = 0;

	virtual int SetColor(PlayParam* lpPlayParam, int nSaturation, int nBrightness, int nContrast, int nHue) = 0;
	virtual int GetColor(PlayParam* lpPlayParam, int* nSaturation, int* nBrightness, int* nContrast, int* nHue) = 0;

	virtual int GetPlayedTime(PlayParam* lpPlayParam) = 0;
	virtual int GetTotalTime(PlayParam* lpPlayParam) = 0;

	virtual int GetPlayedFrame(PlayParam* lpPlayParam) = 0;
	virtual int GetTotalFrame(PlayParam* lpPlayParam) = 0;

	//virtual int GetPicture(PlayParam* lpPlayParam, int* nWidth, int* nHeight) = 0;
	virtual int GetRate(PlayParam* lpPlayParam, int* nRate) = 0;
	virtual int GetBitrate(PlayParam* lpPlayParam, double* dbBitrate) = 0;

	virtual BOOL SnapPicture(PlayParam* lpPlayParam, char* szFileName, int nType) = 0;/*nType,0 bmp 1 jpg*/
	virtual int SetDisplayRegion(PlayParam* lpPlayParam, DWORD nRegionNum, DISPLAYRECT *pSrcRect, HWND hDestWnd, BOOL bEnable) = 0;
	virtual int GetPicSize(PlayParam *lpPlayParam, long *width, long *height) = 0;
	virtual BOOL SetCurrentFrameNum(PlayParam *lpPlayParam, int nFrameNum) = 0;
	virtual BOOL SetPlayedTimeEx(PlayParam *lpPlayParam, int nTtime) = 0;
	virtual BOOL GetNextKeyFramePos(PlayParam *lpPlayParam, DWORD nValue, int nType, PFRAME_POS pFramePos) = 0;
	virtual BOOL GetKeyFramePos(PlayParam *lpPlayParam, DWORD nValue, int nType, PFRAME_POS pFramePos) = 0;
	virtual	BOOL SetVerticalSync(PlayParam *lpPlayParam, BOOL bVerticalSync) = 0;
	virtual BOOL GetQueryInfo(PlayParam *lpPlayParam, int cmdType, char* buf, int buflen, int* returnlen) = 0;
	virtual BOOL RegistDrawCallback(PlayParam *lpPlayParam, fDrawCBFun DrawCBFun, void* nUser) = 0;
	virtual BOOL SetPlayDirection(PlayParam* lpPlayParam, DWORD emDirection) = 0;
	virtual BOOL RenderPrivateData(PlayParam* lpPlayParam, BOOL bTrue) = 0;
	virtual BOOL SetIvsEnable(PlayParam* lpPlayParam, int nIvsType, BOOL bEnable) = 0;
	virtual BOOL StartFisheye(PlayParam* lpPlayParam) = 0;
	virtual BOOL OptFisheyeParams(PlayParam* lpPlayParam, FISHEYE_OPERATETYPE operatetype, FISHEYE_OPTPARAM* pOptParam) = 0;
	virtual BOOL FisheyeEptzUpdate(PlayParam* lpPlayParam, FISHEYE_EPTZPARAM* pEptzParam, BOOL bSecondRegion) = 0;
	virtual BOOL StopFisheye(PlayParam* lpPlayParam) = 0;
	virtual BOOL SetFishEyeInfoCallBack(PlayParam* lpPlayParam, fFishEyeInfoFun pFishEyeInfoFun, void* pUserData) = 0;
	virtual BOOL EnableLargePicAdjustment(PlayParam* lpPlayParam, BOOL bEnable) = 0;
	virtual	BOOL SetAntiAliasing(PlayParam *lpPlayParam, BOOL bAntiAliasing) = 0;
	virtual BOOL SetTranslateString(const char* language) = 0;

};

#endif // !defined(AFX_IPLAY_H__BEF5C3AA_222F_426B_89C2_97CAD97340D6__INCLUDED_)
