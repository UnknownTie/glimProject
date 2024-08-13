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
	//�̹��� Box
	CRect viewBox;
	CRect viewBoxSize;
	// �̹��� ���
	void DisplayImage(CStatic& imgView);
	void DisplayImage() { DisplayImage(pictureCtrl); }
	// �̹��� ����
	IplImage* ImgCombine(IplImage* img1, IplImage* img2, POINT pos);
	// �̹��� ������ ����
	IplImage* ResizeImg(IplImage* img, CRect rect);

	//���
	IplImage* imgBackground = nullptr;
	void UpLoadImg();

	//��
	IplImage* imgCircle = nullptr;
	IplImage* imgSave = nullptr;
	POINT posCircle; //�� ��� ��ǥ 
	void CreateCircle();
	void PosCircle(POINT pos);
	//���ڰ� 0 : ���� ��ǥ , 1 : �� ��ǥ 
	bool bTypeCk;
	POINT posCross[2]; //���ڰ� ��� ��ǥ 
	IplImage* imgCross[2] ; //���ڰ� �̹��� 2���� �����ϰ� ��� ��ġ ����
public:
	void ActionImg();
	void SaveImg(IplImage* img ,int index);
private:
	void createCross();


};
