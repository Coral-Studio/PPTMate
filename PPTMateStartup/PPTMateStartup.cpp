//Use TDM-GCC 9.2 to compile this file

#include <windows.h>
#include <iostream>
#include <fstream>
#include "RegOpt.h"
using namespace std;
inline string GetFileFolderFromPath(string path){
	int index = path.find_last_of('\\');
	string res = path.substr(0, index);
	return res;
}

inline string GetExecFilePath(){
    char _path[256];
    GetModuleFileName(NULL, _path, 256);
    return _path;
}

int main(){
	ofstream logfile("runtime.log");
	clog.rdbuf(logfile.rdbuf());

    string PPTMatePath = GetFileFolderFromPath(GetExecFilePath()) + "\\PPTMate.exe";
    clog << "PPTMatePath: " << PPTMatePath << endl;

    string RegPath = "\"" + PPTMatePath + "\" \"%1\"";
    clog << "RegPath: " << RegPath << endl;

    WriteReg((HKEY)HKEY_CURRENT_USER, (TCHAR*)"SOFTWARE\\Microsoft\\Office\\16.0\\PowerPoint\\Options", (TCHAR*)"AlertIfNotDefault", (DWORD)0);
    // TCHAR val[256];
    // ReadReg((HKEY)HKEY_LOCAL_MACHINE, (TCHAR*)"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\powerpnt.exe", (TCHAR*)"", val);
    DelReg(HKEY_CLASSES_ROOT, (TCHAR*)"PowerPoint.Show.8\\shell\\Open\\command", (TCHAR*)"command");
    WriteReg(HKEY_CLASSES_ROOT, (TCHAR*)"PowerPoint.Show.8\\shell\\Open\\command", (TCHAR*)"", (TCHAR*)RegPath.c_str());
    DelReg(HKEY_CLASSES_ROOT, (TCHAR*)"PowerPoint.Show.12\\shell\\Open\\command", (TCHAR*)"command");
    WriteReg(HKEY_CLASSES_ROOT, (TCHAR*)"PowerPoint.Show.12\\shell\\Open\\command", (TCHAR*)"", (TCHAR*)RegPath.c_str());
    return 0;
}