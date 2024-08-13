#pragma once
#include "linkOpencv.h"
#include "FileManager.h"


class ImgManager {
public:
	ImgManager(CStatic& pictureControl);
	~ImgManager();

private:
	FileManager fileCtrl;
	CStatic& pictureCtrl ;

public:
	void Wait(DWORD mSec);
	//이미지 Box
	CRect viewBox;
	CRect viewBoxSize;
	// 이미지 출력
	void DisplayImage(CStatic& imgView);
	void DisplayImage() { DisplayImage(pictureCtrl); }
	// 이미지 결합
	IplImage* ImgCombine(IplImage* img1, IplImage* img2, POINT pos);
	// 이미지 사이즈 변경
	IplImage* ResizeImg(IplImage* img, CRect rect);

	//배경
	IplImage* imgBackground = nullptr;
	void UpLoadImg();

	//원
	IplImage* imgCircle = nullptr;
	IplImage* imgSave = nullptr;
	POINT posCircle; //원 출력 좌표 
	void CreateCircle();
	void PosCircle(POINT pos);
	//십자가 0 : 시작 좌표 , 1 : 끝 좌표 
	bool bTypeCk;
	POINT posCross[2]; //십자가 출력 좌표 
	IplImage* imgCross[2] ; //십자가 이미지 2개를 생성하고 출력 위치 변경
public:
	void ActionImg();
	void SaveImg(IplImage* img ,int index);
private:
	void createCross();


};
