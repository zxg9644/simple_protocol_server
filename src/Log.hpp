#ifndef __LOG_HPP
#define __LOG_HPP

#include "Singleton.hpp"

class CLog
{
public:
	CLog();

	int Init(char* szLogFilePath, int iLogLevel = 0);//default is debug level
	int DoLog(int iLogLevel, char* szFileName, int iLine, char* szFunction, char* szLogStr, ...);

public:
	static const int LEVEL_DEBUG = 0;
	static const int LEVEL_ERROR = 1;
	static const int LEVEL_INFO = 2;

private:
	int m_iLogLevel;
	FILE* m_pstFile;
};


#define LOG_DEBUG(szLogStr, ...) {CSingleton<CLog>::Instance()->Dolog(CLog::LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, szLogStr, __VA_ARGS__);}

#define LOG_INFO(szLogStr, ...) {CSingleton<CLog>::Instance()->Dolog(CLog::LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, szLogStr, __VA_ARGS__);}

#define LOG_ERROR(szLogStr, ...) {CSingleton<CLog>::Instance()->Dolog(CLog::LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, szLogStr, __VA_ARGS__);}

#endif
