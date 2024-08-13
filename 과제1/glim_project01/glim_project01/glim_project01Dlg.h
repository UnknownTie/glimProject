
// glim_project01Dlg.h: 헤더 파일
//
#pragma once
#include "./ImgManager.h"

// Cglimproject01Dlg 대화 상자
class Cglimproject01Dlg : public CDialogEx
{
// 생성입니다.
public:
	Cglimproject01Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIM_PROJECT01_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
protected:
	CFont m_font;
	CEdit m_txt;
public:// 기본 설정 
	void Init_data();

public:	//이미지 
	ImgManager* mimg = nullptr;
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtTest01();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CEdit m_start;
	CEdit m_end;
	CStatic m_imgViewer;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStnClickedStaticImgview();
	afx_msg void OnBnClickedBtDraw();
	afx_msg void OnBnClickedBtAction();
	afx_msg void OnBnClickedBtOpen();
};
