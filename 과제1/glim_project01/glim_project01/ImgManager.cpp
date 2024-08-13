#include "pch.h"
#include "ImgManager.h"
#include <iostream> 

#define CROSS_SIZE 11

ImgManager::ImgManager(CStatic& pictureControl) : pictureCtrl(pictureControl)
{
	//기본 설정
	pictureCtrl.GetClientRect(&viewBox);
	viewBoxSize = viewBox;

	//기본 배경 이미지
	IplImage* bkImg = cvLoadImage("../img/Lenna.png", CV_LOAD_IMAGE_UNCHANGED);
	imgBackground = ResizeImg(bkImg, viewBoxSize);
	cvReleaseImage(&bkImg);

	//십자가 
	createCross();
	bTypeCk = false;
	posCross[0] = { viewBox.left + (viewBox.Width() / 5), viewBox.top + (viewBox.Height() / 5) };
	posCross[1] = { viewBox.right - (viewBox.Width() / 5), viewBox.bottom - (viewBox.Height() / 5) };

	//원 
	CreateCircle();
	PosCircle(posCross[0]);
	//출력
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

//이미지 출력 
void ImgManager::DisplayImage( CStatic& pictureControl)
{
	IplImage* Img = cvCreateImage(cvSize(viewBoxSize.Width(), viewBoxSize.Height()), IPL_DEPTH_8U, 4);

	//이미지 결합 
	if (imgBackground != nullptr)
		cvCopy(imgBackground, Img);
	if (imgCircle != nullptr)
		ImgCombine(Img, imgCircle , posCircle);
	if (imgCross[0] != nullptr)
		ImgCombine(Img, imgCross[0], posCross[0]);
	if (imgCross[1] != nullptr)
		ImgCombine(Img, imgCross[1], posCross[1]);

	//이미지 출력 
	CDC* pDC = pictureControl.GetDC();
	CRect rect;
	pictureControl.GetClientRect(&rect);

	// BITMAPINFO 설정
	BITMAPINFO bitmapInfo;
	memset(&bitmapInfo, 0, sizeof(bitmapInfo));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = Img->width;
	bitmapInfo.bmiHeader.biHeight = -Img->height; // 상하 반전
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32; // 4채널(RGBA) 이미지
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	// 이미지 데이터를 픽처 컨트롤에 출력
	::SetDIBitsToDevice(
		pDC->GetSafeHdc(),             // 디바이스 컨텍스트의 핸들
		0, 0,                          // 출력 시작 지점
		Img->width, Img->height,       // 출력 이미지의 크기
		0, 0,                          // 이미지의 원점
		0,                             // 이미지의 첫 번째 스캔 라인
		Img->height,                   // 이미지의 총 스캔 라인 수
		Img->imageData,                // 이미지 데이터
		&bitmapInfo,                   // BITMAPINFO 구조체
		DIB_RGB_COLORS                 // 색상 사용
	);

	// DC 해제
	pictureControl.ReleaseDC(pDC);

	cvReleaseImage(&Img);
}

//이미지 결합
IplImage* ImgManager::ImgCombine(IplImage* img1, IplImage* img2, POINT pos)
{
	if (img1 == nullptr || img2 == nullptr)
		return img1;

	// 합성을 시작할 위치 계산
	int startX = pos.x < 0 ? 0 : pos.x;
	int startY = pos.y < 0 ? 0 : pos.y;

	// img2의 시작 위치 계산 (음수 pos를 생략하기 위함)
	int offsetX = pos.x < 0 ? -pos.x : 0;
	int offsetY = pos.y < 0 ? -pos.y : 0;

	// img2를 img1의 지정된 위치에 합성
	for (int y = 0; y < img2->height - offsetY; ++y) {
		for (int x = 0; x < img2->width - offsetX; ++x) {
			int overlayPixelIndex = (y + offsetY) * img2->widthStep + (x + offsetX) * img2->nChannels;
			int srcPixelIndex = (y + startY) * img1->widthStep + (x + startX) * img1->nChannels;

			if (srcPixelIndex >= 0 && (x + startX) < img1->width && (y + startY) < img1->height) {
				uchar* overlayPixel = &((uchar*)img2->imageData)[overlayPixelIndex];
				uchar* srcPixel = &((uchar*)img1->imageData)[srcPixelIndex];

				
				if (overlayPixel[3] != 0 ) {//투명 제외 
					if (overlayPixel[0] != 255 || overlayPixel[1] != 255 || overlayPixel[2] != 255) { //완전 흰색 제외
						for (int i = 0; i < img1->nChannels; ++i) {
							srcPixel[i] = overlayPixel[i];
						}
					}else {
						//완전 흰색인 영역은 이미지 저장
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

// 이미지 사이즈 변경
IplImage* ImgManager::ResizeImg(IplImage* img, CRect rect)
{
	// 이미지의 원본 크기
	int orgWidth = img->width;
	int orgHeight = img->height;

	// Rect의 크기
	int targetWidth = rect.Width();
	int targetHeight = rect.Height();

	// 가로세로 비율 유지한 상태로 크기 조정
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

	// 이미지 크기 조정
	IplImage* resizedImg = cvCreateImage(cvSize(newWidth, newHeight), img->depth, img->nChannels);
	cvResize(img, resizedImg);

	// 전체 크기의 흰색 배경 이미지 생성
	IplImage* rtnImg = cvCreateImage(cvSize(targetWidth, targetHeight), img->depth, img->nChannels);
	cvSet(rtnImg, cvScalar(255, 255, 255 ,255)); // 흰색으로 채우기

	// resizedImg를 outputImg의 중앙에 배치
	int xOffset = (targetWidth - newWidth) / 2;
	int yOffset = (targetHeight - newHeight) / 2;
	cvSetImageROI(rtnImg, cvRect(xOffset, yOffset, newWidth, newHeight));
	cvCopy(resizedImg, rtnImg);
	cvResetImageROI(rtnImg);

	// 크기 조정된 이미지 해제
	cvReleaseImage(&resizedImg);

	return rtnImg;
}

//이미지 가져오기
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

//원  생성
void ImgManager::CreateCircle() {
	//최대는 전체 사이즈의 3분의1 
	int maxRendom = viewBox.Width() / 3;
	//최대는 전체 사이즈의 6분의1 
	int minRendom = viewBox.Width() / 6;
	int nRandSize = rand() % maxRendom + minRendom + 1;

	if (nRandSize % 2 != 0)
		nRandSize++;

	//이미지 저장용
	imgSave = cvCreateImage(cvSize(nRandSize, nRandSize), IPL_DEPTH_8U, 4);
	cvSetZero(imgSave);

	//원 이미지 생성
	imgCircle = cvCreateImage(cvSize(nRandSize, nRandSize), IPL_DEPTH_8U, 4);
	cvSetZero(imgCircle);

	cvCircle(imgCircle, cvPoint(nRandSize / 2, nRandSize / 2), nRandSize / 2, cvScalar(255, 255, 255,255), -1);
	cvCircle(imgCircle, cvPoint(nRandSize / 2, nRandSize / 2), nRandSize / 2, cvScalar(0, 0, 0, 255), 1);
}

//좌표 변환
void ImgManager::PosCircle(POINT pos) {

	posCircle.x = pos.x - (imgCircle->width / 2) + (CROSS_SIZE / 2);
	posCircle.y = pos.y - (imgCircle->height / 2) + (CROSS_SIZE / 2);
}


//십자가
void ImgManager::createCross() {
	//start
	// 빨간색 십자가
	imgCross[0] = cvCreateImage(cvSize(CROSS_SIZE, CROSS_SIZE), IPL_DEPTH_8U, 4);
	cvSetZero(imgCross[0]);
	cvLine(imgCross[0], cvPoint(0, CROSS_SIZE /2 ), cvPoint(CROSS_SIZE, CROSS_SIZE / 2), cvScalar(0, 0, 255, 255), 2);
	cvLine(imgCross[0], cvPoint(CROSS_SIZE / 2, 0), cvPoint(CROSS_SIZE / 2, CROSS_SIZE), cvScalar(0, 0, 255, 255),2);

	//end 
	// 파란색 십자가
	imgCross[1] = cvCreateImage(cvSize(CROSS_SIZE, CROSS_SIZE), IPL_DEPTH_8U,4);
	cvSetZero(imgCross[1]);
	cvLine(imgCross[1], cvPoint(0, CROSS_SIZE / 2), cvPoint(CROSS_SIZE, CROSS_SIZE / 2),  cvScalar(255, 0, 0 ,255), 2);
	cvLine(imgCross[1], cvPoint(CROSS_SIZE / 2, 0), cvPoint(CROSS_SIZE / 2, CROSS_SIZE),  cvScalar(255, 0, 0 ,255), 2);
}

//이동하면서 이미지 저장
void ImgManager::ActionImg() {
	POINT start = posCross[0];
	POINT end = posCross[1];
	PosCircle(start);
	// 각 축의 거리 차이 계산
	int dx = (end.x - start.x) / 5;
	int dy = (end.y - start.y) / 5;

	// 현재 위치를 시작점으로 초기화
	POINT currentPoint = start;

	// 5회 이동
	for (int i = 0; i <= 5; i++) {
		DisplayImage();
		SaveImg(imgSave, i);
		Wait(300);
		currentPoint.x += dx;
		currentPoint.y += dy;
		PosCircle(currentPoint);
	}
}

// JPG 파일로 저장
void ImgManager::SaveImg(IplImage* img , int index) {
	
	CString strFolder = fileCtrl.GetExePath();
	strFolder += _T("/image/") + fileCtrl.GetDay() + _T("/");
	//폴더 확인
	fileCtrl.CreateFolder(strFolder);
	CString strFilename;
	strFilename.Format(_T("%s_img_%d.jpg"), fileCtrl.GetTime() , index);
	cv::Mat matImg = cv::cvarrToMat(img);

	imwrite(fileCtrl.CStr2str(strFolder + strFilename), matImg);
}

//sleep 기능 
void ImgManager::Wait(DWORD ms) // 밀리초 
{
	MSG msg;
	DWORD  dwStart = GetTickCount();

	while ((GetTickCount() - dwStart) < ms)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ //메시지큐 확인
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


