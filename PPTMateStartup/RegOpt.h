// refer: https://blog.csdn.net/woniu211111/article/details/82496817

#ifndef REGOPT_H_
#define REGOPT_H_

#include <Windows.h>
#include <iostream>

using std::clog, std::endl;

int ReadReg(HKEY rootKey, TCHAR *regPath, TCHAR *regKey, TCHAR *regValue)
{
	clog << "ReadReg " << rootKey << "\\" << regPath << " [" << (!strcmp(regKey, (TCHAR*)"") ? (TCHAR*)"(Default)" : regKey) << "]" << endl;

	HKEY hKey;
	int ret = RegOpenKeyEx(rootKey, regPath, 0, KEY_EXECUTE, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		clog << "[Error] RegOpenKeyEx failed" << endl;
		return 1;
	}

	DWORD dwType = REG_SZ;
	DWORD cbData = 256;
	ret = RegQueryValueEx(hKey, regKey, NULL, &dwType, (LPBYTE)regValue, &cbData);
	if (ret == ERROR_SUCCESS)
	{
		clog << "[Info] ReadReg successfully, [" << (!strcmp(regKey, (TCHAR*)"") ? (TCHAR*)"(Default)" : regKey) << " = " << regValue << "]" << endl;
		RegCloseKey(hKey);
		return 0;
	}
	else
	{
		clog << "[Error] RegQueryValueEx failed" << endl;
		RegCloseKey(hKey);
		return 1;
	}
}

int ReadReg(HKEY rootKey, TCHAR *regPath, TCHAR *regKey, DWORD regValue)
{
	clog << "ReadReg " << rootKey << "\\" << regPath << " [" << (!strcmp(regKey, (TCHAR*)"") ? (TCHAR*)"(Default)" : regKey) << "]" << endl;

	HKEY hKey;
	int ret = RegOpenKeyEx(rootKey, regPath, 0, KEY_EXECUTE, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		clog << "[Error] RegOpenKeyEx failed" << endl;
		return 1;
	}

	DWORD dwType = REG_DWORD;
	DWORD cbData = sizeof(DWORD);
	ret = RegQueryValueEx(hKey, regKey, NULL, &dwType, (LPBYTE)&regValue, &cbData);
	if (ret == ERROR_SUCCESS)
	{
		clog << "[Info] ReadReg successfully, [" << (!strcmp(regKey, (TCHAR*)"") ? (TCHAR*)"(Default)" : regKey) << " = " << regValue << "]" << endl;
		RegCloseKey(hKey);
		return 0;
	}
	else
	{
		clog << "[Error] RegQueryValueEx failed" << endl;
		RegCloseKey(hKey);
		return 1;
	}
}

int WriteReg(HKEY rootKey, TCHAR *regPath, TCHAR *regKey, TCHAR *regValue)
{
	clog << "WriteReg " << rootKey << "\\" << regPath << " [" << (!strcmp(regKey, (TCHAR*)"") ? (TCHAR*)"(Default)" : regKey) << " = " << regValue << "]" << endl;

	HKEY hKey;
	DWORD dwDisp;
	DWORD dwType = REG_SZ;

	int ret = RegCreateKeyEx(rootKey, regPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
	if (ret != ERROR_SUCCESS)
	{
		clog << "[Error] RegCreateKeyEx failed" << endl;
		return 1;
	}
	ret = RegSetValueEx(hKey, regKey, 0, dwType, (LPBYTE)regValue, strlen(regValue));
	if (ret != ERROR_SUCCESS)
	{
		clog << "[Error] RegSetValueEx failed" << endl;
		RegCloseKey(hKey);
		return 1;
	}
	RegCloseKey(hKey);
	return 0;
}

int WriteReg(HKEY rootKey, TCHAR *regPath, TCHAR *regKey, DWORD regValue)
{
	clog << "WriteReg " << rootKey << "\\" << regPath << " [" << (!strcmp(regKey, (TCHAR*)"") ? (TCHAR*)"(Default)" : regKey) << " = " << regValue << "]" << endl;

	HKEY hKey;
	DWORD dwDisp;
	DWORD dwType = REG_DWORD;

	int ret = RegCreateKeyEx(rootKey, regPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
	if (ret != ERROR_SUCCESS)
	{
		clog << "[Error] RegCreateKeyEx failed" << endl;
		return 1;
	}
	ret = RegSetValueEx(hKey, regKey, 0, dwType, (LPBYTE)&regValue, sizeof(DWORD));
	if (ret != ERROR_SUCCESS)
	{
		clog << "[Error] RegSetValueEx failed" << endl;
		RegCloseKey(hKey);
		return 1;
	}
	RegCloseKey(hKey);
	return 0;
}

int DelReg(HKEY rootKey, TCHAR *regPath)
{
	clog << "DelReg " << rootKey << "\\" << regPath << endl;

	int ret = RegDeleteKey(rootKey, regPath);
	if (ret != ERROR_SUCCESS)
	{
		clog << "[Error] RegDeleteKey failed" << endl;
		return 1;
	}
	return 0;
}

int DelReg(HKEY rootKey, TCHAR *regPath, TCHAR *regKey)
{
	clog << "DelReg " << rootKey << "\\" << regPath << " [" << (!strcmp(regKey, (TCHAR*)"") ? (TCHAR*)"(Default)" : regKey) << "]" << endl;

	HKEY hKey = NULL;
	LONG ret = RegOpenKeyEx(rootKey, regPath, 0, KEY_ALL_ACCESS, &hKey);
	if (ret == ERROR_SUCCESS)
	{
		ret = RegDeleteValue(hKey, regKey);
		if (ret == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return 0;
		}
		else
		{
			clog << "[Error] RegDeleteValue failed" << endl;
			RegCloseKey(hKey);
			return 1;
		}
	}
	else
	{
		clog << "[Error] RegOpenKeyEx failed" << endl;
		RegCloseKey(hKey);
		return 1;
	}
}

#endif