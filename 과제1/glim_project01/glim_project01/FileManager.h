#pragma once
#include <string>    
#include <atlstr.h> 

using namespace std;

class FileManager {
public:
	CString GetExePath();
	bool CreateFolder(CString dirPath);
	CString GetTime();
	CString GetDay();

public:
	// CString -> string
	string CStr2str(CString cstr) {return string(CT2A(cstr));};

public:
	CString dirFolder;
	CString file;


};