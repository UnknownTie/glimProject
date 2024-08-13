#include "pch.h"
#include "FileManager.h"
#include <direct.h>


// 현재 시간 ( 년 월 일 )
CString FileManager::GetDay() {
	CTime time = CTime::GetCurrentTime();
	CString strFilename;
	strFilename.Format(_T("%04d%02d%02d"),
		time.GetYear(), time.GetMonth(), time.GetDay());

	return strFilename;
}
// 현재 시간 ( 시 분 초 )
CString FileManager::GetTime() {
	CTime time = CTime::GetCurrentTime();
	CString strFilename;
	strFilename.Format(_T("%02d%02d%02d"),
		time.GetHour(), time.GetMinute(), time.GetSecond());

	return strFilename;
}

// 파일 실행 경로 확인 
CString FileManager::GetExePath() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	CString exePath(buffer);
	int pos = exePath.ReverseFind('\\');
	if (pos != -1) 
		exePath = exePath.Left(pos);
	
	return exePath;
}

bool FileManager::CreateFolder(CString dirPath) {
	CString path = dirPath;
	CStringArray dirList;
	int pos = 0;

	// 전체 경로 분리
	while ((pos = path.FindOneOf(_T("\\/"))) != -1) {
		CString part = path.Left(pos);
		if (!part.IsEmpty())
			dirList.Add(part);
		path = path.Mid(pos + 1);
	}
	if (!path.IsEmpty())
		dirList.Add(path);

	// 전체 디렉토리 체크 및 생성 
	CString currentPath;
	for (int i = 0; i < dirList.GetSize(); i++) {
		currentPath += dirList[i];
		if (_mkdir(CT2A(currentPath)) != 0 && errno != EEXIST)
			return false;
		currentPath += "\\";
	}

	return true;
}


