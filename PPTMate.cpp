#include <windows.h>
#include <iostream>
#include <atlstr.h>

using namespace std;

CString PowerPnt_Path,
		ArchiveFolder_Path;

int IsCreateSubfolder,
	IsPowerpntPathOverride;

static int ReadReg(HKEY rootKey, TCHAR* regPath, TCHAR* regKey, TCHAR* regValue)
{
	HKEY hKey;
	int ret = RegOpenKeyEx(rootKey, regPath, 0, KEY_EXECUTE, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		cout << "[Error] RegOpenKeyEx failed" << endl;
		return 1;
	}

	DWORD dwType = REG_SZ;
	DWORD cbData = 256;
	ret = RegQueryValueEx(hKey, regKey, NULL, &dwType, (LPBYTE)regValue, &cbData);
	if (ret == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 0;
	}
	else
	{
		cout << "[Error] RegQueryValueEx failed" << endl;
		RegCloseKey(hKey);
		return 1;
	}
}

static inline BOOL IsPathExist(const CString& csPath)
{
	int nRet = _taccess(csPath, 0);
	return 0 == nRet || EACCES == nRet;
}

static inline CString GetFileNameFromPath(CString path) {
	int index = path.ReverseFind('\\');
	return path.Right(path.GetLength() - (index + 1));
}

static inline CString GetFileFolderFromPath(CString path) {
	int index = path.ReverseFind('\\');
	return path.Left(index);
}

static inline CString GetTimeStr() {
	SYSTEMTIME currentTime;
	char res[20];

	GetLocalTime(&currentTime);
	snprintf(res, 20, "%d-%d-%d", currentTime.wYear, currentTime.wMonth, currentTime.wDay);

	return res;
}

int main(int argc, char* argv[]) {
	std::cout << "Debug Info:" << std::endl << std::endl;

	//Preload configures
	CString cwdPath;
	GetModuleFileName(NULL, cwdPath.GetBuffer(MAX_PATH), MAX_PATH);
	//wprintf(L"path:%s\n", szPath.GetString());
	SetCurrentDirectory(GetFileFolderFromPath(cwdPath));

	CString tempIsCreateSubfolder, tempIsPowerpntPathOverride;
	GetPrivateProfileString(_T("Path"), _T("powerpoint"), _T("C:\\Program Files\\Microsoft Office\\root\\Office16\\POWERPNT.EXE"), PowerPnt_Path.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\config.ini"));
	GetPrivateProfileString(_T("Path"), _T("powerpoint_path_override"), _T("false"), tempIsPowerpntPathOverride.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\config.ini"));
	GetPrivateProfileString(_T("Path"), _T("archive_folder"), _T("F:\\PPTBACKUP"), ArchiveFolder_Path.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\config.ini"));
	GetPrivateProfileString(_T("Setting"), _T("is_create_subfolder"), _T("true"), tempIsCreateSubfolder.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\config.ini"));
	if (tempIsPowerpntPathOverride == "true") IsPowerpntPathOverride = 1;
	else IsPowerpntPathOverride = 0;
	if (tempIsCreateSubfolder == "true") IsCreateSubfolder = 1;
	else IsCreateSubfolder = 0;

	if(IsPowerpntPathOverride == 0)
	{
		TCHAR val[256];
    	ReadReg((HKEY)HKEY_LOCAL_MACHINE, CA2T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\powerpnt.exe"), CA2T(""), val);
		PowerPnt_Path = val;
		wprintf(L"%s\n", PowerPnt_Path.GetString());
	}

	if (!IsPathExist(PowerPnt_Path))
	{
		wprintf(L"[Error] Powerpoint executable file (%s) not vaild\n", PowerPnt_Path.GetString());
		MessageBox(NULL, L"Powerpoint可执行文件路径无效。\n请先打开Powerpoint应用程序并在程序中打开文件。", L"PPTMate无法启动Powerpoint", MB_OK | MB_ICONWARNING);
		return 0;
	}

	if (argc < 2) return 0;

	CString filePath, fileName, subfolderName, destFolderPath, destFilePath;

	filePath = argv[1];
	fileName = GetFileNameFromPath(filePath);
	subfolderName = GetTimeStr();
	if (IsCreateSubfolder) destFolderPath.Format(L"%s\\%s", ArchiveFolder_Path.GetString(), subfolderName.GetString());
	else destFolderPath = ArchiveFolder_Path;
	destFilePath.Format(L"%s\\%s", destFolderPath.GetString(), fileName.GetString());


	wprintf(L"%s %s %s %s %s\n",
		filePath.GetString(),
		fileName.GetString(),
		subfolderName.GetString(),
		destFolderPath.GetString(),
		destFilePath.GetString()
	);

	CreateDirectory(ArchiveFolder_Path.GetString(), NULL);
	if (IsCreateSubfolder) CreateDirectory(destFolderPath.GetString(), NULL);

	CopyFile(filePath.GetString(), destFilePath.GetString(), FALSE);

	CString curCmd;
	curCmd.Format(L"/c cd /d \"%s\" && start %s \"%s\"", GetFileFolderFromPath(PowerPnt_Path).GetString(), GetFileNameFromPath(PowerPnt_Path).GetString(), filePath.GetString());
	wprintf(L"%s\n", curCmd.GetString());
	auto ret = ShellExecute(
		NULL,
		CA2T("open"),
		CA2T("cmd.exe"),
		curCmd.GetString(),
		NULL,
		SW_HIDE
	);
	if ((INT_PTR)ret <= 32)
	{
		printf("[Error] Failed to open Powerpoint");
		MessageBox(NULL, L"Powerpoint可执行文件路径无效。\n请先打开Powerpoint应用程序并在程序中打开文件。", L"PPTMate无法启动Powerpoint", MB_OK | MB_ICONWARNING);
		return 0;
	}
	return 0;
}
