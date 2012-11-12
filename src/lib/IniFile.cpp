#include "IniFile.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

IniFile::IniFile( )
{

}

IniFile::~IniFile()
{

}

bool IniFile::Init( char* szFileName )
{
	if (NULL == szFileName || strlen(szFileName) == 0)
	{
		return false;
	}

	m_strFileName = szFileName;

	FILE* pFile = fopen( szFileName, "rb" );
	if (NULL == pFile)
	{
		return false;
	}

	char szReadLineBuf[MAX_LINE_BUF_SIZE];
	char szLineBuf[MAX_LINE_BUF_SIZE];
	string strCurSection;
	string strKey;
	string strValue;
	while(NULL != fgets(szReadLineBuf, MAX_LINE_BUF_SIZE, pFile))
	{
		DelInvalidSign(szReadLineBuf, szLineBuf);

		if (IsNoteLine(szLineBuf) || IsEmptyLine(szLineBuf))
		{
			continue;
		}
		else if (IsNewSection(szLineBuf))
		{
			GetNewSectionContext(szLineBuf, strCurSection);
		}
		else if (IsKeyValueLine(szLineBuf))
		{
			GetKeyValue(szLineBuf, strKey, strValue);
			m_mapSection[strCurSection][strKey] = strValue;
		}
		else
		{
			continue;
		}
	}

	return true;
}

bool IniFile::IsNoteLine( char* szLine )
{
	return (szLine[0] == '#');
}

bool IniFile::IsEmptyLine( char* szLine )
{
	int nLineSize = strlen(szLine);

	if (nLineSize == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IniFile::IsNewSection( char* szLine )
{
	return (strchr(szLine, '[') && strchr(szLine, ']'));
}

bool IniFile::IsKeyValueLine( char* szLine )
{
	return (NULL != strchr(szLine, '='));
}

bool IniFile::GetNewSectionContext( char* szLine, string& strNewSectionContext )
{
	strNewSectionContext.clear();

	char* pSectionContextBegin = strchr(szLine, '[');
	char* pSectionContextEnd = strchr(szLine, ']');
	int nSectionContextLen = pSectionContextEnd - pSectionContextBegin - 1;

	char szSectionContextBuf[nSectionContextLen];
	memcpy(szSectionContextBuf, pSectionContextBegin + 1, nSectionContextLen);
	strNewSectionContext = szSectionContextBuf;

	return true;
}

bool IniFile::GetKeyValue( char* szLine, string& strKey, string& strValue )
{
	strKey.clear();
	strValue.clear();

	char* pEqualPos = strchr(szLine, '=');
	int nKeyLen = pEqualPos - szLine;
	int nValueLen = strlen(szLine) - nKeyLen - 1;

	char szKeyBuf[nKeyLen];
	memcpy(szKeyBuf, szLine, nKeyLen);
	strKey = szKeyBuf;

	char szValueBuf[nValueLen];
	memcpy(szValueBuf, pEqualPos + 1, nValueLen);
	strValue = szValueBuf;

	return true;
}

void IniFile::Save()
{

}

bool IniFile::SaveAs( char* szFileName )
{
	if (NULL == szFileName || strlen(szFileName) == 0)
	{
		return false;
	}

	FILE* pFile = fopen(szFileName, "w");
	if (NULL == pFile)
	{
		return false;
	}

	// 写入文件内容
	MapSection::iterator itSection = m_mapSection.begin();
	for (; itSection != m_mapSection.end(); itSection++)
	{
	
	}
	return true;
}

void IniFile::ShowFileContext()
{
	MapSection::iterator itSection = m_mapSection.begin();
	for (; itSection != m_mapSection.end(); itSection++)
	{
		MapKeyValue& refKeyValueMap = itSection->second;
		cout << "==============================" << endl;
		cout << "Section:" << itSection->first << endl;

		MapKeyValue::iterator itKV = refKeyValueMap.begin();
		for (; itKV != refKeyValueMap.end(); itKV++)
		{
			cout << itKV->first << " = " << itKV->second << endl;
		}
		cout << "==============================" << endl;
		cout << endl;
	}
}

void IniFile::DelInvalidSign( char* szOldLine, char* szNewLine )
{
	int iOldLineLen;
	if (NULL == szOldLine || (iOldLineLen = strlen(szOldLine)) == 0)
	{
		return;
	}

	char tmpChar;
	int nNewLineIndex = 0;
	for (int i = 0; i < iOldLineLen; i++)
	{
		tmpChar = szOldLine[i];
		if (tmpChar == ' '
			            || tmpChar == '\t'
			            || tmpChar == '\r'
			|| tmpChar == '\n')
		{
			continue;
		}

		szNewLine[nNewLineIndex++] = tmpChar;
	}

	szNewLine[nNewLineIndex] = 0;
}

std::string IniFile::GetValue( const string& strKey )
{
	MapSection::iterator itSection = m_mapSection.begin();
	for (; itSection != m_mapSection.end(); itSection++)
	{
		MapKeyValue& refKeyValueMap = itSection->second;
		MapKeyValue::iterator itKV = refKeyValueMap.find(strKey);
		if (itKV != refKeyValueMap.end())
		{
			return itKV->second;
		}
	}

	return "";
}

std::string IniFile::GetValueFromSection( const string& strSection, const string& strKey )
{
	MapSection::iterator itSection = m_mapSection.find(strSection);
	if (itSection == m_mapSection.end())
	{
		return "";
	}

	MapKeyValue& refKeyValueMap = itSection->second;
	MapKeyValue::iterator itKV = refKeyValueMap.find(strKey);
	if (itKV != refKeyValueMap.end())
	{
		return itKV->second;
	}

	return "";
}

int IniFile::GetInt( const string& strKey )
{
	string str = GetValue(strKey);

	return atoi(str.c_str());
}

float IniFile::GetFloat( const string& strKey )
{
	string str = GetValue(strKey);

	return atof(str.c_str());
}		