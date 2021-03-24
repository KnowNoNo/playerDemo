// stdafx.cpp : source file that includes just the standard includes
//	PlayDemo.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include "CharactorTansfer.h"
//AnsiתUTF-8
std::string AnsiToUtf8(const std::string& strAnsi)//�����strAnsi��Ansi����
{
	//Ansiתunicode
	int len = MultiByteToWideChar(CP_ACP, 0, strAnsi.c_str(), -1, NULL, 0);
	wchar_t *strUnicode = new wchar_t[len];
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_ACP, 0, strAnsi.c_str(), -1, strUnicode, len);

	//unicodeתUTF-8
	len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	char * strUtf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, strUtf8, len, NULL, NULL);

	std::string strTemp(strUtf8);//��ʱ��strTemp��UTF-8����
	delete[]strUnicode;
	delete[]strUtf8;
	strUnicode = NULL;
	strUtf8 = NULL;
	return strTemp;
}

//UTF-8תAnsi
std::string Utf8ToAnsi(const std::string& strUtf8)//�����strUtf8��UTF-8����
{
	//UTF-8תunicode
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
	wchar_t * strUnicode = new wchar_t[len];//len = 2
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, strUnicode, len);

	//unicodeתAnsi
	len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	char *strAnsi = new char[len];//len=3 ����Ϊ2������char*�����Զ�������\0
	memset(strAnsi, 0, len);
	WideCharToMultiByte(CP_ACP,0, strUnicode, -1, strAnsi, len, NULL, NULL);

	std::string strTemp(strAnsi);//��ʱ��strTemp��Ansi����
	delete[]strUnicode;
	delete[]strAnsi;
	strUnicode = NULL;
	strAnsi = NULL;
	return strTemp;
}

//Ansiתunicode (���������û�õ�)
std::wstring AnsiToUnicode(const std::string& strAnsi)//����ֵ��wstring
{
	int len = MultiByteToWideChar(CP_ACP, 0, strAnsi.c_str(), -1, NULL, 0);
	wchar_t *strUnicode = new wchar_t[len];
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_ACP, 0, strAnsi.c_str(), -1, strUnicode, len);

	std::wstring strTemp(strUnicode);//��ʱ��strTemp��Unicode����
	delete[]strUnicode;
	strUnicode = NULL;
	return strTemp;
}

//UnicodeתAnsi
std::string UnicodeToAnsi (const std::wstring& strUnicode)//������wstring
{
	int len = WideCharToMultiByte(CP_ACP, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	char *strAnsi = new char[len];//len=3 ����Ϊ2������char*�����Զ�������\0
	memset(strAnsi, 0, len);
	WideCharToMultiByte(CP_ACP,0,strUnicode.c_str(), -1, strAnsi, len, NULL, NULL);
	
	std::string strTemp(strAnsi);
	delete []strAnsi;
	strAnsi = NULL;
	return strTemp;
}
