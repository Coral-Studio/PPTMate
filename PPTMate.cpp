#include<iostream>
#include<cstdio>
#include<string>
#include<windows.h>

using namespace std;

const string pptexe_path = "\"C:\\Program Files\\Microsoft Office\\root\\Office16\"";
const string pptexe_name = "POWERPNT.EXE";
const string des_path = "F:\\PPTBACKUP\\";

string file_path, sub_folder;

inline static void DoCmd(string cmd) {
	// cout<<"> "<<cmd<<endl;
	WinExec(("cmd.exe /c " + cmd).c_str(), SW_HIDE);
	Sleep(100);
}

inline string GetTimeStr() {
	SYSTEMTIME tt;
	char res[20];

	GetLocalTime(&tt);
	snprintf(res, 20, "%d-%d-%d", tt.wYear, tt.wMonth, tt.wDay);

	string str = res;
	return str;
}

int main(int argc, char* argv[]) {
	if (argc < 2) return 0;

	file_path = argv[1];
	file_path = "\"" + file_path + "\"";

	sub_folder = GetTimeStr();

	DoCmd("cd /d " + des_path + " && mkdir " + sub_folder);
	DoCmd("copy " + file_path + " " + des_path + sub_folder);
	DoCmd("cd /d " + pptexe_path + " && start " + pptexe_name + " " + file_path);

	return 0;
}
