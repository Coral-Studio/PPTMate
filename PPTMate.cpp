#include <windows.h>
#include <iostream>
#include <string>
#include <atlstr.h>

using namespace std;

CString PowerPnt_Path,
		ArchiveFolder_Path;

int IsCreateSubfolder;


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

inline CString GetFileFolderFromPath(string path) {
	int index = path.find_last_of('\\');
	string res = path.substr(0, index);
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
	CString cwdPath;
	GetModuleFileName(NULL, cwdPath.GetBuffer(MAX_PATH), MAX_PATH);
	//wprintf(L"path:%s\n", szPath.GetString());
	SetCurrentDirectory(GetFileFolderFromPath((string)(CT2A(cwdPath))));

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
	destFilePath.Format(L"%s\\%s", destFolderPath.GetString(), fileName.GetString());


	wprintf(L"%s %s %s %s %s\n",
		filePath.GetString(),
		fileName.GetString(),
		subfolderName.GetString(),
		destFolderPath.GetString(),
		destFilePath.GetString()
	);

	CString curCmd;

	CreateDirectory(ArchiveFolder_Path.GetString(), NULL);
	if (IsCreateSubfolder) CreateDirectory(destFolderPath.GetString(), NULL);
	CopyFile(filePath.GetString(), destFilePath.GetString(), FALSE);
	//curCmd.Format(L"cmd.exe /c mkdir \"%s\"", destFolderPath.GetString());
	//WinExec(CT2A(curCmd), SW_HIDE);
	//wprintf(L"%s\n", curCmd.GetString());

	CopyFile(filePath.GetString(), destFilePath.GetString(), FALSE);
	//curCmd.Format(L"cmd.exe /c copy %s %s", filePath.GetString(), destFolderPath.GetString());
	//WinExec(CT2A(curCmd), SW_HIDE);
	//wprintf(L"%s\n", curCmd.GetString());

	curCmd.Format(L"cmd.exe /c cd /d \"%s\" && start %s \"%s\"", GetFileFolderFromPath((string)(CT2A(PowerPnt_Path))).GetString(), GetFileNameFromPath((string)(CT2A(PowerPnt_Path))).GetString(), filePath.GetString());
	WinExec(CT2A(curCmd), SW_HIDE);
	//wprintf(L"%s\n", curCmd.GetString());

	return 0;
}
