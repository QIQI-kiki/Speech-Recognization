// StenotypistDlg.h : header file
//

#if !defined(AFX_STENOTYPISTDLG_H__11C37955_805B_43B2_A11D_6A0E14104216__INCLUDED_)
#define AFX_STENOTYPISTDLG_H__11C37955_805B_43B2_A11D_6A0E14104216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStenotypistDlg dialog

#include "CSpeechRecognition.h"

class CStenotypistDlg : public CDialog
{
// Construction
public:
	CStenotypistDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CStenotypistDlg)
	enum { IDD = IDD_STENOTYPIST_DIALOG };
	CButton	m_btDictation;
	CString	m_strText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStenotypistDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CStenotypistDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTONDictate();
	afx_msg void OnBUTTONVt();
	afx_msg void OnBUTTONMs();
	afx_msg LRESULT OnSREvent(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CSpeechRecognition	m_SpeechRecognition;
	BOOL fir;
public:
	afx_msg void OnCbnSelchangeComboSr();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STENOTYPISTDLG_H__11C37955_805B_43B2_A11D_6A0E14104216__INCLUDED_)
