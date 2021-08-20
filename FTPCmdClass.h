#pragma once

#include "PublicFunction.h"

#define MAX_SEND_SIZE		 1024			//255+strlen("SIZE ");
#define MAX_RECV_CMD_SIZE    1024
#define ONCE_RECV_DATA_SIZE  4096
#define NET_BUFFER_SIZE		 4096			// Ĭ�ϻ����С

class CFTPCmdClass
{
public:
	CFTPCmdClass(void);
	virtual ~CFTPCmdClass(void);

public:
	SOCKET m_socketCtrl;					// ����������׽���
	SOCKET m_socketData;					// ���ݴ���

	int     m_nPort;						// �˿ں�
	CString m_csIp;							// ip
	CString m_csUsername;					// �û���
	CString m_csPassword;					// ����

	bool	m_bIsConnect;					// �Ƿ�����
	bool    m_bIsPort;						// �Ƿ�����ģʽ ��Ĭ�ϱ���ģʽ��
	bool    m_bIsForce;						// �Ƿ����´��� ��Ĭ�϶ϵ�������
	bool    m_bSupportResume;				// �Ƿ�֧�ֶϵ�����
	CString csLog;							// log��־  
	CString m_csLine;						// ����������
	char*   m_chLine;						// ����������
	CByteArray m_buf;						// ���ջ�������
	CString m_csRetMsg;						// ����������Ϣ
	int nResponseCode;						// ��Ӧ��		
	CFile*  m_pFile;						// �ϴ��ļ�
	HANDLE  m_hEvtEndModule;				// �����¼�
public:
	BOOL LogOn(CString szFtpName,int nPort,CString szFtpUser,CString szFtpPassword);						// ��¼��ip���˿ںţ��û��������룩
	BOOL LogOff();																							// �˳�
	// �ϴ��ļ� ������Ŀ¼���ļ����������ļ���С��������Ŀ¼��������ͬ���ļ���С�������ֽڣ������¼����Ƿ�����ģʽ���Ƿ��ش���
	BOOL UploadFile(CString csLocDir,CString csLocFileName,ULONGLONG ullLocFileSize,CString csServDir,ULONGLONG* pullServFileSize,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule,bool bIsPort=false,bool bIsForce=false);
	BOOL DownloadFile(CString csLocDir,CString csServFileName,ULONGLONG* pullLocFileSize,CString csServDir,ULONGLONG* pullServFileSize,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule,bool bIsPort=false,bool bIsForce=false);

	BOOL CreateDir(CString csDirName);																		// �������Ŀ¼
	BOOL DeleteDirOrFile(CString csName , UINT uMark);														// ���ɾ��Ŀ¼
	BOOL List(CString &csFTPDir);																			// ����õ��ļ��б�
	BOOL GetFileItem(int& nIndex);																			// �����ļ��б�
	BOOL GetCurrentDir(CString &csFTPDir);																	// ����õ���ǰĿ¼

	void SetEventOfEndModule(HANDLE hEvtEndModule);															// ���ý����¼�

protected:
	BOOL Open(CString csHost,int nPort,SOCKET &socketCtrl,BOOL bSetKeepAlive = TRUE);						// ��socket
	void Close(SOCKET &socketCtrl);																			// �ر�socket
	BOOL Send(CString csSend);																				// ��������
	int  Response();																						// �ӷ���������ͨ����ȡһ����Ӧ
	int  GetDigitStrAtHead (CString csStr);																	// �������еõ���Ӧ��
	BOOL CreateDataConnect (SOCKET &socketData);															// �����������ӣ��������ݴ��䣩
	BOOL GetIPAndPortByPasvString ( LPCTSTR lpszPasvString, CString &csIP, int &nPort );					// �������н���IP,�˿ں�
	LONGLONG ReceiveCtrlData(SOCKET &socketRecv);															// �ӿ���ͨ���������ݲ�����m_buf

	BOOL SendData (CFile* file,SOCKET &socketSend,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule);		// �ϴ�����
	BOOL ReceiveData(CFile* file,SOCKET &socketReceive,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule); // ��������

	BOOL SetKeepAlive(SOCKET socket_handle);																// ���ô�����
	BOOL OpenFile(CFile* file,CString csFileName,BOOL bUpload,ULONGLONG& nStartPos);						// ���ļ�

public:

};
