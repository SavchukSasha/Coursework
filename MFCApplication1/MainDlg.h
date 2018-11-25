
// MainDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CMaimDlg dialog
class CMainDlg : public CDialog
{
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString text1;
	afx_msg void OnBnClickedButton2();
	CString text2;
	afx_msg void OnBnClickedButton3();
	CString text3;
	CString text4;
	CString in;

	int num_cilk = 10;
	int mas[10] = {10, 20, 30, 80, 60, 655, 2000, 300, 232, 666};
	int mas_poch[10] = {0};
	int num_error;
	int err = 1;
	CString text_error;	
};
