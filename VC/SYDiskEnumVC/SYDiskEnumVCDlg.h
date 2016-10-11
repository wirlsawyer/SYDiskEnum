// SYDiskEnumVCDlg.h : header file
//

#pragma once


// CSYDiskEnumVCDlg dialog
class CSYDiskEnumVCDlg : public CDialog
{
// Construction
public:
	CSYDiskEnumVCDlg(CWnd* pParent = NULL);	// standard constructor
	CListBox *mListBox;
// Dialog Data
	enum { IDD = IDD_SYDISKENUMVC_DIALOG };

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
};
