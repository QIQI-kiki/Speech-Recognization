// Stenotypist.h : main header file for the STENOTYPIST application
//

#if !defined(AFX_STENOTYPIST_H__2D068A8F_1A8B_4633_9C51_558EF7C8E0B1__INCLUDED_)
#define AFX_STENOTYPIST_H__2D068A8F_1A8B_4633_9C51_558EF7C8E0B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStenotypistApp:
// See Stenotypist.cpp for the implementation of this class
//

class CStenotypistApp : public CWinApp
{
public:
	CStenotypistApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStenotypistApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStenotypistApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STENOTYPIST_H__2D068A8F_1A8B_4633_9C51_558EF7C8E0B1__INCLUDED_)
