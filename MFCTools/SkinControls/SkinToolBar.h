#ifndef SKIN_TOOLBAR_HEAD_FILE
#define SKIN_TOOLBAR_HEAD_FILE

#pragma once

#include "SkinControlsHead.h"
//////////////////////////////////////////////////////////////////////////


#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class SKIN_CONTROLS_CLASS CSkinToolBar : public CToolBar
{
	//��������
protected:
	struct tagDropDownInfo {
	public:
		UINT  uButtonID;
		UINT  uMenuID;
		CWnd* pParent;
	};
	BOOL m_bDropDown;
	CArray<tagDropDownInfo, tagDropDownInfo&> m_DropDownButton;
	//��������
public:
	//���캯��
	CSkinToolBar();
	//��������
	~CSkinToolBar();
	
// Operations
public:
	BOOL SetToolBarButton(INT nBtnWidth, UINT uToolBar,
							  UINT uToolBarHot = 0,
							  UINT uToolBarDisabled = 0);
	void AddDropDownButton(CWnd* pParent, UINT uButtonID, UINT uMenuID);

private:
	BOOL SetToolBar(UINT uToolBarType, UINT uToolBar, INT nBtnWidth);


protected:
	//{{AFX_MSG(CTrueColorToolBar)
	afx_msg void OnToolbarDropDown(NMHDR* pnmh, LRESULT* plRes);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
