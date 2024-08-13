
// glim_project01Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "glim_project01.h"
#include "glim_project01Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cglimproject01Dlg 대화 상자



Cglimproject01Dlg::Cglimproject01Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIM_PROJECT01_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cglimproject01Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TXT_START, m_start);
	DDX_Control(pDX, IDC_TXT_END, m_end);
	DDX_Control(pDX, IDC_STATIC_IMGVIEW, m_imgViewer);

	
}

BEGIN_MESSAGE_MAP(Cglimproject01Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_TEST01, &Cglimproject01Dlg::OnBnClickedBtTest01)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_STN_CLICKED(IDC_STATIC_IMGVIEW, &Cglimproject01Dlg::OnStnClickedStaticImgview)
	ON_BN_CLICKED(IDC_BT_DRAW, &Cglimproject01Dlg::OnBnClickedBtDraw)
	ON_BN_CLICKED(IDC_BT_ACTION, &Cglimproject01Dlg::OnBnClickedBtAction)
	ON_BN_CLICKED(IDC_BT_OPEN, &Cglimproject01Dlg::OnBnClickedBtOpen)
END_MESSAGE_MAP()


// Cglimproject01Dlg 메시지 처리기

BOOL Cglimproject01Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_font.CreatePointFont(90, _T("Arial"));
	// 다이얼로그와 모든 자식 컨트롤에 폰트를 설정합니다.
	SetFont(&m_font, TRUE);

	// 모든 컨트롤에 대해 폰트 적용
	CWnd* pWnd = GetWindow(GW_CHILD);
	while (pWnd) {
		pWnd->SetFont(&m_font, TRUE);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Cglimproject01Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cglimproject01Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		Init_data();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Cglimproject01Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cglimproject01Dlg::Init_data() {
	//초기값 설정 
	mimg = new ImgManager(m_imgViewer);
	//좌표값 설정
	CRect ViewMapSize;
	m_imgViewer.GetWindowRect(&ViewMapSize);
	ScreenToClient(&ViewMapSize);
	mimg->viewBox.left = ViewMapSize.left;
	mimg->viewBox.top = ViewMapSize.top;
	mimg->viewBox.right = mimg->viewBox.left + mimg->imgBackground->width;
	mimg->viewBox.bottom = mimg->viewBox.left + mimg->imgBackground->height;

	// 좌표 표시
	CString str;
	str.Format(_T("(X : %d ,Y : %d)"), mimg->posCross[0].x , mimg->posCross[0].y);
	m_start.SetWindowText(str);
	str.Format(_T("(X : %d ,Y : %d)"), mimg->posCross[1].x, mimg->posCross[1].y);
	m_end.SetWindowText(str);

}

void Cglimproject01Dlg::OnBnClickedBtTest01()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Cglimproject01Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//범위 안으로 들어오면 값 삽입 
	if (mimg->viewBox.PtInRect(point))
	{
		// 좌표 offset 
		CPoint localPoint = point;
		localPoint.Offset(-mimg->viewBox.left, -mimg->viewBox.top);

		// 마우스 클릭 좌표 기준
		CString str;
		if (!mimg->bTypeCk) {
			//시작점
			mimg->posCross[0] = localPoint;
			str.Format(_T("(X : %d ,Y : %d)"), mimg->posCross[0].x, mimg->posCross[0].y);
			m_start.SetWindowText(str);
		}
		else {
			//끝점
			mimg->posCross[1] = localPoint;
			str.Format(_T("(X : %d ,Y : %d)"), mimg->posCross[1].x, mimg->posCross[1].y);
			m_end.SetWindowText(str);
		}

		mimg->bTypeCk = !mimg->bTypeCk;
		mimg->DisplayImage();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void Cglimproject01Dlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void Cglimproject01Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CString str;
	//str.Format(_T("(X : %d ,Y : %d)"), point.x, point.y);
	//m_start.SetWindowText(str);

	CDialogEx::OnMouseMove(nFlags, point);
}


void Cglimproject01Dlg::OnStnClickedStaticImgview()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mimg->CreateCircle();
	mimg->PosCircle(mimg->posCross[0]);

	mimg->DisplayImage();
}


void Cglimproject01Dlg::OnBnClickedBtDraw()
{
	// 3. Draw 버튼 클릭시 (x1, y1) 좌표를 중심으로하는 랜덤한 크기의 원을 그립니다.
	mimg->CreateCircle();
	mimg->PosCircle(mimg->posCross[0]);
	mimg->DisplayImage();
}


void Cglimproject01Dlg::OnBnClickedBtAction()
{
	// 4. 4. Action 버튼 클릭시 (x1, y1) 좌표에서 (x2, y2) 좌표로 일정 픽셀 간격으로 원을 이동시키며, 
	//이동할 때마다 실행파일이 위치한 폴더 내의 image 폴더에 bmp 또는 jpg(jpeg) 포맷으로 저장합니다.

	// 버튼을 비활성화 ( 중복 방지 ) 
	GetDlgItem(IDC_BT_ACTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_DRAW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_OPEN)->EnableWindow(FALSE);
	//시작 -> 끝 (이동횟수는 6회로 지정)
	mimg->ActionImg();

	// 버튼을 활성화
	GetDlgItem(IDC_BT_ACTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_BT_DRAW)->EnableWindow(TRUE);
	GetDlgItem(IDC_BT_OPEN)->EnableWindow(TRUE);
}


void Cglimproject01Dlg::OnBnClickedBtOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mimg->UpLoadImg();
	mimg->DisplayImage();
}
