// SYDiskEnumVCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SYDiskEnumVC.h"
#include "SYDiskEnumVCDlg.h"
#include "DiskEnum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSYDiskEnumVCDlg dialog




CSYDiskEnumVCDlg::CSYDiskEnumVCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSYDiskEnumVCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSYDiskEnumVCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSYDiskEnumVCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSYDiskEnumVCDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSYDiskEnumVCDlg message handlers

BOOL CSYDiskEnumVCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	mListBox = (CListBox*)GetDlgItem(IDC_LIST1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSYDiskEnumVCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSYDiskEnumVCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSYDiskEnumVCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSYDiskEnumVCDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//clear listbox	
	mListBox->SetCurSel(-1);
	mListBox->ResetContent();


	DiskEnum *disknum = new DiskEnum();
	disknum->Scan();
	std::vector<DiskInfo> *pInfo = disknum->GetDiskInfo();
	std::vector<DiskInfo>::iterator iter = pInfo->begin();
	while (iter != pInfo->end())
	{
		DiskInfo item = *iter;

		mListBox->AddString((item.Mount+L":\\").c_str());		
		mListBox->AddString((item.FriendlyName).c_str());
		mListBox->AddString((item.InstanceID).c_str());
		mListBox->AddString((item.DiskID).c_str());
		mListBox->AddString((item.Volume).c_str());
		mListBox->AddString((item.ParentDriver).c_str());
		mListBox->AddString(L"--------------------------------------");

		
		OutputDebugString((item.Mount+L":\\").c_str());		
		OutputDebugString(L"\n");
		OutputDebugString((item.FriendlyName).c_str());
		OutputDebugString(L"\n");
		OutputDebugString((item.InstanceID).c_str());
		OutputDebugString(L"\n");
		OutputDebugString((item.DiskID).c_str());
		OutputDebugString(L"\n");
		OutputDebugString((item.Volume).c_str());
		OutputDebugString(L"\n");
		OutputDebugString((item.ParentDriver).c_str());
		OutputDebugString(L"\n");
		OutputDebugString(L"--------------------------------------");
		OutputDebugString(L"\n");
		iter++;
	}
	delete disknum;
}
