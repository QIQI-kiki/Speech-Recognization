// StenotypistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Stenotypist.h"
#include "StenotypistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStenotypistDlg dialog

CStenotypistDlg::CStenotypistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStenotypistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStenotypistDlg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStenotypistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStenotypistDlg)
	DDX_Control(pDX, IDC_BUTTON_Dictate, m_btDictation);
	DDX_Text(pDX, IDC_STATIC_SR, m_strText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStenotypistDlg, CDialog)
	//{{AFX_MSG_MAP(CStenotypistDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Dictate, OnBUTTONDictate)
	ON_BN_CLICKED(IDC_BUTTON_Vt, OnBUTTONVt)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON_Ms, OnBUTTONMs)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SREVENT, OnSREvent)
	ON_CBN_SELCHANGE(IDC_COMBO_SR, &CStenotypistDlg::OnCbnSelchangeComboSr)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStenotypistDlg message handlers

BOOL CStenotypistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	fir=TRUE;
	m_SpeechRecognition.Initialize(this->m_hWnd,IDC_COMBO_SR);
	m_SpeechRecognition.Stop();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStenotypistDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStenotypistDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CStenotypistDlg::OnBUTTONDictate() 
{
	CString s;
	((CButton *)GetDlgItem(IDC_BUTTON_Dictate))->GetWindowText(s);

	if(s=="听写(&D)"){
		m_SpeechRecognition.SetVoice(this->m_hWnd,IDC_COMBO_SR);
		((CWnd *)(this->GetDlgItem(IDC_COMBO_SR)))->EnableWindow(FALSE);
		((CWnd *)GetDlgItem(IDC_BUTTON_Dictate))->SetWindowText("停止(&S)");

		if(fir){
				((CWnd *)GetDlgItem(IDC_STATIC_STATE))->SetWindowText("状态：配置向导....");
		int u=MessageBox("这也许是您第一次使用语音识别，强烈建议您运行配置向导以配置相关选项。\n是否运行配置向导？","语音识别",MB_YESNO|MB_ICONQUESTION);		
			if(u==IDYES) 
			{
				m_SpeechRecognition.MicrophoneSetup(m_hWnd);
				m_SpeechRecognition.VoiceTraining(m_hWnd);
			}
		}
		((CWnd *)GetDlgItem(IDC_STATIC_STATE))->SetWindowText("状态：听写....");
		m_SpeechRecognition.Start();
	}else{
		((CWnd *)GetDlgItem(IDC_STATIC_STATE))->SetWindowText("状态：就绪");
		((CWnd *)(this->GetDlgItem(IDC_COMBO_SR)))->EnableWindow(TRUE);
		((CWnd *)GetDlgItem(IDC_BUTTON_Dictate))->SetWindowText("听写(&D)");
		m_SpeechRecognition.Stop();
	}
}

void CStenotypistDlg::OnBUTTONVt() 
{
	// TODO: Add your control notification handler code here
	m_SpeechRecognition.VoiceTraining(m_hWnd);
}

void CStenotypistDlg::OnBUTTONMs() 
{
	// TODO: Add your control notification handler code here
	m_SpeechRecognition.MicrophoneSetup(m_hWnd);
}
 
LRESULT CStenotypistDlg::OnSREvent(WPARAM, LPARAM)
{	
	WCHAR *pwzText;
	m_SpeechRecognition.GetText(&pwzText);	
	m_strText += CString(pwzText);
	UpdateData(FALSE);
	return 0L;
}


void CStenotypistDlg::OnCbnSelchangeComboSr()
{
	// TODO: 在此添加控件通知处理程序代码
}
