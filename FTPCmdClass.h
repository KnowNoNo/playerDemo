#pragma once

#include "PublicFunction.h"

#define MAX_SEND_SIZE		 1024			//255+strlen("SIZE ");
#define MAX_RECV_CMD_SIZE    1024
#define ONCE_RECV_DATA_SIZE  4096
#define NET_BUFFER_SIZE		 4096			// 默认缓冲大小

class CFTPCmdClass
{
public:
	CFTPCmdClass(void);
	virtual ~CFTPCmdClass(void);

public:
	SOCKET m_socketCtrl;					// 控制命令传输套接字
	SOCKET m_socketData;					// 数据传输

	int     m_nPort;						// 端口号
	CString m_csIp;							// ip
	CString m_csUsername;					// 用户名
	CString m_csPassword;					// 密码

	bool	m_bIsConnect;					// 是否连接
	bool    m_bIsPort;						// 是否主动模式 （默认被动模式）
	bool    m_bIsForce;						// 是否重新传输 （默认断点续传）
	bool    m_bSupportResume;				// 是否支持断点续传
	CString csLog;							// log日志  
	CString m_csLine;						// 接收命令行
	char*   m_chLine;						// 接收命令行
	CByteArray m_buf;						// 接收缓存数据
	CString m_csRetMsg;						// 接收命令信息
	int nResponseCode;						// 回应码		
	CFile*  m_pFile;						// 上传文件
	HANDLE  m_hEvtEndModule;				// 结束事件
public:
	BOOL LogOn(CString szFtpName,int nPort,CString szFtpUser,CString szFtpPassword);						// 登录（ip，端口号，用户名，密码）
	BOOL LogOff();																							// 退出
	// 上传文件 （本地目录，文件名，本地文件大小，服务器目录，服务器同名文件大小，传输字节，结束事件，是否主动模式，是否重传）
	BOOL UploadFile(CString csLocDir,CString csLocFileName,ULONGLONG ullLocFileSize,CString csServDir,ULONGLONG* pullServFileSize,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule,bool bIsPort=false,bool bIsForce=false);
	BOOL DownloadFile(CString csLocDir,CString csServFileName,ULONGLONG* pullLocFileSize,CString csServDir,ULONGLONG* pullServFileSize,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule,bool bIsPort=false,bool bIsForce=false);

	BOOL CreateDir(CString csDirName);																		// 命令：创建目录
	BOOL DeleteDirOrFile(CString csName , UINT uMark);														// 命令：删除目录
	BOOL List(CString &csFTPDir);																			// 命令：得到文件列表
	BOOL GetFileItem(int& nIndex);																			// 解析文件列表
	BOOL GetCurrentDir(CString &csFTPDir);																	// 命令：得到当前目录

	void SetEventOfEndModule(HANDLE hEvtEndModule);															// 设置结束事件

protected:
	BOOL Open(CString csHost,int nPort,SOCKET &socketCtrl,BOOL bSetKeepAlive = TRUE);						// 打开socket
	void Close(SOCKET &socketCtrl);																			// 关闭socket
	BOOL Send(CString csSend);																				// 发送命令
	int  Response();																						// 从服务器控制通道获取一行响应
	int  GetDigitStrAtHead (CString csStr);																	// 从命令行得到回应码
	BOOL CreateDataConnect (SOCKET &socketData);															// 创建数据连接（用于数据传输）
	BOOL GetIPAndPortByPasvString ( LPCTSTR lpszPasvString, CString &csIP, int &nPort );					// 从命令行解析IP,端口号
	LONGLONG ReceiveCtrlData(SOCKET &socketRecv);															// 从控制通道接收数据并保存m_buf

	BOOL SendData (CFile* file,SOCKET &socketSend,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule);		// 上传数据
	BOOL ReceiveData(CFile* file,SOCKET &socketReceive,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule); // 下载数据

	BOOL SetKeepAlive(SOCKET socket_handle);																// 设置存活机制
	BOOL OpenFile(CFile* file,CString csFileName,BOOL bUpload,ULONGLONG& nStartPos);						// 打开文件

public:

};
