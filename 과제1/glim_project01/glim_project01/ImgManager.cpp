#include "pch.h"
#include "ImgManager.h"
#include <iostream> 

#define CROSS_SIZE 11

ImgManager::ImgManager(CStatic& pictureControl) : pictureCtrl(pictureControl)
{
	//�⺻ ����
	pictureCtrl.GetClientRect(&viewBox);
	viewBoxSize = viewBox;

	//�⺻ ��� �̹���
	IplImage* bkImg = cvLoadImage("../img/Lenna.png", CV_LOAD_IMAGE_UNCHANGED);
	imgBackground = ResizeImg(bkImg, viewBoxSize);
	cvReleaseImage(&bkImg);

	//���ڰ� 
	createCross();
	bTypeCk = false;
	posCross[0] = { viewBox.left + (viewBox.Width() / 5), viewBox.top + (viewBox.Height() / 5) };
	posCross[1] = { viewBox.right - (viewBox.Width() / 5), viewBox.bottom - (viewBox.Height() / 5) };

	//�� 
	CreateCircle();
	PosCircle(posCross[0]);
	//���
	DisplayImage();
}

ImgManager::~ImgManager()
{
	if (imgBackground != nullptr)
		cvReleaseImage(&imgBackground);
	if (imgCircle != nullptr)
		cvReleaseImage(&imgCircle);
	if (imgSave != nullptr)
		cvReleaseImage(&imgSave);
	if (imgCross[0] != nullptr)
		cvReleaseImage(&imgCross[0]);
	if (imgCross[1] != nullptr)
		cvReleaseImage(&imgCross[1]);
	
}

//�̹��� ��� 
void ImgManager::DisplayImage( CStatic& pictureControl)
{
	IplImage* Img = cvCreateImage(cvSize(viewBoxSize.Width(), viewBoxSize.Height()), IPL_DEPTH_8U, 4);

	//�̹��� ���� 
	if (imgBackground != nullptr)
		cvCopy(imgBackground, Img);
	if (imgCircle != nullptr)
		ImgCombine(Img, imgCircle , posCircle);
	if (imgCross[0] != nullptr)
		ImgCombine(Img, imgCross[0], posCross[0]);
	if (imgCross[1] != nullptr)
		ImgCombine(Img, imgCross[1], posCross[1]);

	//�̹��� ��� 
	CDC* pDC = pictureControl.GetDC();
	CRect rect;
	pictureControl.GetClientRect(&rect);

	// BITMAPINFO ����
	BITMAPINFO bitmapInfo;
	memset(&bitmapInfo, 0, sizeof(bitmapInfo));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = Img->width;
	bitmapInfo.bmiHeader.biHeight = -Img->height; // ���� ����
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32; // 4ä��(RGBA) �̹���
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	// �̹��� �����͸� ��ó ��Ʈ�ѿ� ���
	::SetDIBitsToDevice(
		pDC->GetSafeHdc(),             // ����̽� ���ؽ�Ʈ�� �ڵ�
		0, 0,                          // ��� ���� ����
		Img->width, Img->height,       // ��� �̹����� ũ��
		0, 0,                          // �̹����� ����
		0,                             // �̹����� ù ��° ��ĵ ����
		Img->height,                   // �̹����� �� ��ĵ ���� ��
		Img->imageData,                // �̹��� ������
		&bitmapInfo,                   // BITMAPINFO ����ü
		DIB_RGB_COLORS                 // ���� ���
	);

	// DC ����
	pictureControl.ReleaseDC(pDC);

	cvReleaseImage(&Img);
}

//�̹��� ����
IplImage* ImgManager::ImgCombine(IplImage* img1, IplImage* img2, POINT pos)
{
	if (img1 == nullptr || img2 == nullptr)
		return img1;

	// �ռ��� ������ ��ġ ���
	int startX = pos.x < 0 ? 0 : pos.x;
	int startY = pos.y < 0 ? 0 : pos.y;

	// img2�� ���� ��ġ ��� (���� pos�� �����ϱ� ����)
	int offsetX = pos.x < 0 ? -pos.x : 0;
	int offsetY = pos.y < 0 ? -pos.y : 0;

	// img2�� img1�� ������ ��ġ�� �ռ�
	for (int y = 0; y < img2->height - offsetY; ++y) {
		for (int x = 0; x < img2->width - offsetX; ++x) {
			int overlayPixelIndex = (y + offsetY) * img2->widthStep + (x + offsetX) * img2->nChannels;
			int srcPixelIndex = (y + startY) * img1->widthStep + (x + startX) * img1->nChannels;

			if (srcPixelIndex >= 0 && (x + startX) < img1->width && (y + startY) < img1->height) {
				uchar* overlayPixel = &((uchar*)img2->imageData)[overlayPixelIndex];
				uchar* srcPixel = &((uchar*)img1->imageData)[srcPixelIndex];

				
				if (overlayPixel[3] != 0 ) {//���� ���� 
					if (overlayPixel[0] != 255 || overlayPixel[1] != 255 || overlayPixel[2] != 255) { //���� ��� ����
						for (int i = 0; i < img1->nChannels; ++i) {
							srcPixel[i] = overlayPixel[i];
						}
					}else {
						//���� ����� ������ �̹��� ����
						uchar* savePixel = &((uchar*)imgSave->imageData)[overlayPixelIndex];
						for (int i = 0; i < imgSave->nChannels; ++i)
							savePixel[i] = srcPixel[i];
					}
				}
			}
		}
	}
	return img1;
}

// �̹��� ������ ����
IplImage* ImgManager::ResizeImg(IplImage* img, CRect rect)
{
	// �̹����� ���� ũ��
	int orgWidth = img->width;
	int orgHeight = img->height;

	// Rect�� ũ��
	int targetWidth = rect.Width();
	int targetHeight = rect.Height();

	// ���μ��� ���� ������ ���·� ũ�� ����
	float aspectRatio = (float)orgWidth / (float)orgHeight;
	int newWidth, newHeight;

	if (orgWidth > orgHeight) {
		newWidth = targetWidth;
		newHeight = static_cast<int>(targetWidth / aspectRatio);
		if (newHeight > targetHeight) {
			newHeight = targetHeight;
			newWidth = static_cast<int>(targetHeight * aspectRatio);
		}
	} else {
		newHeight = targetHeight;
		newWidth = static_cast<int>(targetHeight * aspectRatio);
		if (newWidth > targetWidth) {
			newWidth = targetWidth;
			newHeight = static_cast<int>(targetWidth / aspectRatio);
		}
	}

	// �̹��� ũ�� ����
	IplImage* resizedImg = cvCreateImage(cvSize(newWidth, newHeight), img->depth, img->nChannels);
	cvResize(img, resizedImg);

	// ��ü ũ���� ��� ��� �̹��� ����
	IplImage* rtnImg = cvCreateImage(cvSize(targetWidth, targetHeight), img->depth, img->nChannels);
	cvSet(rtnImg, cvScalar(255, 255, 255 ,255)); // ������� ä���

	// resizedImg�� outputImg�� �߾ӿ� ��ġ
	int xOffset = (targetWidth - newWidth) / 2;
	int yOffset = (targetHeight - newHeight) / 2;
	cvSetImageROI(rtnImg, cvRect(xOffset, yOffset, newWidth, newHeight));
	cvCopy(resizedImg, rtnImg);
	cvResetImageROI(rtnImg);

	// ũ�� ������ �̹��� ����
	cvReleaseImage(&resizedImg);

	return rtnImg;
}

//�̹��� ��������
void ImgManager::UpLoadImg() {
	TCHAR szFilter[] = _T("ALL Files(*.*)|*.*|*.bmp |*.bmp| *.jpg |*.jpg| *.gif |*.gif|");
	CFileDialog DIG_IMAGE_R(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilter);
	if (DIG_IMAGE_R.DoModal() == IDOK){

		CString Image_Data;
		Image_Data.Format(_T("%s"), DIG_IMAGE_R.GetPathName());

		CStringA strAFilename(Image_Data);
		const char* filename = strAFilename.GetString();
		IplImage* bkImg = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
		imgBackground = ResizeImg(bkImg, viewBoxSize);
		cvReleaseImage(&bkImg);
	}
}

//��  ����
void ImgManager::CreateCircle() {
	//�ִ�� ��ü �������� 3����1 
	int maxRendom = viewBox.Width() / 3;
	//�ִ�� ��ü �������� 6����1 
	int minRendom = viewBox.Width() / 6;
	int nRandSize = rand() % maxRendom + minRendom + 1;

	if (nRandSize % 2 != 0)
		nRandSize++;

	//�̹��� �����
	imgSave = cvCreateImage(cvSize(nRandSize, nRandSize), IPL_DEPTH_8U, 4);
	cvSetZero(imgSave);

	//�� �̹��� ����
	imgCircle = cvCreateImage(cvSize(nRandSize, nRandSize), IPL_DEPTH_8U, 4);
	cvSetZero(imgCircle);

	cvCircle(imgCircle, cvPoint(nRandSize / 2, nRandSize / 2), nRandSize / 2, cvScalar(255, 255, 255,255), -1);
	cvCircle(imgCircle, cvPoint(nRandSize / 2, nRandSize / 2), nRandSize / 2, cvScalar(0, 0, 0, 255), 1);
}

//��ǥ ��ȯ
void ImgManager::PosCircle(POINT pos) {

	posCircle.x = pos.x - (imgCircle->width / 2) + (CROSS_SIZE / 2);
	posCircle.y = pos.y - (imgCircle->height / 2) + (CROSS_SIZE / 2);
}


//���ڰ�
void ImgManager::createCross() {
	//start
	// ������ ���ڰ�
	imgCross[0] = cvCreateImage(cvSize(CROSS_SIZE, CROSS_SIZE), IPL_DEPTH_8U, 4);
	cvSetZero(imgCross[0]);
	cvLine(imgCross[0], cvPoint(0, CROSS_SIZE /2 ), cvPoint(CROSS_SIZE, CROSS_SIZE / 2), cvScalar(0, 0, 255, 255), 2);
	cvLine(imgCross[0], cvPoint(CROSS_SIZE / 2, 0), cvPoint(CROSS_SIZE / 2, CROSS_SIZE), cvScalar(0, 0, 255, 255),2);

	//end 
	// �Ķ��� ���ڰ�
	imgCross[1] = cvCreateImage(cvSize(CROSS_SIZE, CROSS_SIZE), IPL_DEPTH_8U,4);
	cvSetZero(imgCross[1]);
	cvLine(imgCross[1], cvPoint(0, CROSS_SIZE / 2), cvPoint(CROSS_SIZE, CROSS_SIZE / 2),  cvScalar(255, 0, 0 ,255), 2);
	cvLine(imgCross[1], cvPoint(CROSS_SIZE / 2, 0), cvPoint(CROSS_SIZE / 2, CROSS_SIZE),  cvScalar(255, 0, 0 ,255), 2);
}

//�̵��ϸ鼭 �̹��� ����
void ImgManager::ActionImg() {
	POINT start = posCross[0];
	POINT end = posCross[1];
	PosCircle(start);
	// �� ���� �Ÿ� ���� ���
	int dx = (end.x - start.x) / 5;
	int dy = (end.y - start.y) / 5;

	// ���� ��ġ�� ���������� �ʱ�ȭ
	POINT currentPoint = start;

	// 5ȸ �̵�
	for (int i = 0; i <= 5; i++) {
		DisplayImage();
		SaveImg(imgSave, i);
		Wait(300);
		currentPoint.x += dx;
		currentPoint.y += dy;
		PosCircle(currentPoint);
	}
}

// JPG ���Ϸ� ����
void ImgManager::SaveImg(IplImage* img , int index) {
	
	CString strFolder = fileCtrl.GetExePath();
	strFolder += _T("/image/") + fileCtrl.GetDay() + _T("/");
	//���� Ȯ��
	fileCtrl.CreateFolder(strFolder);
	CString strFilename;
	strFilename.Format(_T("%s_img_%d.jpg"), fileCtrl.GetTime() , index);
	cv::Mat matImg = cv::cvarrToMat(img);

	imwrite(fileCtrl.CStr2str(strFolder + strFilename), matImg);
}

//sleep ��� 
void ImgManager::Wait(DWORD ms) // �и��� 
{
	MSG msg;
	DWORD  dwStart = GetTickCount();

	while ((GetTickCount() - dwStart) < ms)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ //�޽���ť Ȯ��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


