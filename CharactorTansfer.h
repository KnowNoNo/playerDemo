#ifndef CHARACTOR_TANSFER
#define CHARACTOR_TANSFER

#include <string>

std::string UnicodeToAnsi (const std::wstring& strUnicode);//������wstring
std::wstring AnsiToUnicode(const std::string& strAnsi);
#endif