#include <windows.h>
#include <iostream>
#include <string>
#include <atlstr.h>

using namespace std;

CString PowerPnt_Path,
		ArchiveFolder_Path;

int IsCreateSubfolder;


//string WCharToMByte(LPCWSTR lpcwszStr)
//{
//	string str;
//	DWORD dwMinSize = 0;
//	LPSTR lpszStr = NULL;
//	dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, NULL, 0, NULL, FALSE);
//	if (0 == dwMinSize)
//	{
//		return FALSE;
//	}
//	lpszStr = new char[dwMinSize];
//	WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, lpszStr, dwMinSize, NULL, FALSE);
//	str = lpszStr;
//	delete[] lpszStr;
//	return str;
//}

BOOL IsPathExist(const CString& csPath)
{
	int nRet = _taccess(csPath, 0);
	return 0 == nRet || EACCES == nRet;
}

inline CString GetFileNameFromPath(string path) {
	int index = path.find_last_of('\\');
	string res = path.substr(index + 1, path.size() - (index + 1));
	return (CString)res.c_str();
}

inline CString GetTimeStr() {
	SYSTEMTIME currentTime;
	char res[20];

	GetLocalTime(&currentTime);
	snprintf(res, 20, "%d-%d-%d", currentTime.wYear, currentTime.wMonth, currentTime.wDay);

	return res;
}

int main(int argc, char* argv[]) {
	std::cout << "Debug Info:" << std::endl << std::endl;

	//Preload configures
	CString tempIsCreateSubfolder;
	GetPrivateProfileString(_T("Path"), _T("powerpoint"), _T("C:\\Program Files\\Microsoft Office\\root\\Office16\\POWERPNT.EXE"), PowerPnt_Path.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\config.ini"));
	GetPrivateProfileString(_T("Path"), _T("archive_folder"), _T("D:\\PPTBACKUP"), ArchiveFolder_Path.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\config.ini"));
	GetPrivateProfileString(_T("Setting"), _T("is_create_subfolder"), _T("true"), tempIsCreateSubfolder.GetBuffer(MAX_PATH), MAX_PATH, _T(".\\config.ini"));
	if (tempIsCreateSubfolder == "true") IsCreateSubfolder = 1;
	else IsCreateSubfolder = 0;
	wprintf(L"%s %s %s\n", PowerPnt_Path.GetString(), ArchiveFolder_Path.GetString(), tempIsCreateSubfolder.GetString());

	if (!IsPathExist(PowerPnt_Path)) {
		printf("[Error] Powerpoint executable file not vaild\n");
		return 0;
	}

	if (argc < 2) return 0;

	CString filePath, fileName, subfolderName, destFolderPath, destFilePath;

	filePath = argv[1];
	fileName = GetFileNameFromPath((string)(CT2A(filePath)));
	subfolderName = GetTimeStr();
	if (IsCreateSubfolder) destFolderPath.Format(L"%s\\%s", ArchiveFolder_Path.GetString(), subfolderName.GetString());
	else destFolderPath = ArchiveFolder_Path;
	destFilePath.Format(L"%s\\%s", destFolderPath.GetString(),fileName.GetString());


	wprintf(L"%s %s %s %s %s\n",
		filePath.GetString(),
		fileName.GetString(),
		subfolderName.GetString(),
		destFolderPath.GetString(),
		destFilePath.GetString()
	);

	CreateDirectory(ArchiveFolder_Path.GetString(), NULL);
	if(IsCreateSubfolder) CreateDirectory(destFolderPath.GetString(), NULL);
	CopyFile(filePath.GetString(), destFilePath.GetString(), FALSE);
	
	CString curCmd;
	curCmd.Format(L"cmd.exe /c start \"%s\" \"%s\"", PowerPnt_Path.GetString(), filePath.GetString());
	wprintf(L"%s\n", curCmd.GetString());
	WinExec(CT2A(curCmd), SW_HIDE);

	return 0;
}
