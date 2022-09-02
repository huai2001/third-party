#pragma once

#include "resource.h"
#include "StaticColor.h"

// CSettingDialog �Ի���

class CSettingDialog : public CSkinDialog
{
	DECLARE_DYNAMIC(CSettingDialog)

private:
	HWND				m_hFromWnd;
    CSkinEdit           m_EditPosX[7];
    CSkinEdit           m_EditPosY[7];
    CSkinEdit           m_EditColor[7];
    CStaticColor        m_StaticColor[7];

    CSkinButton         m_btOK;
    CSkinButton         m_btCancal;

    //��������
public:
    //���캯��
    CSettingDialog(CWnd* pParent = NULL);
    //��������
    virtual ~CSettingDialog();

// �Ի�������
	enum { IDD = IDD_SETTING_DIALOG };

    void SetFromWnd(HWND hWnd) {
        m_hFromWnd = hWnd;
    }
    //���غ���
public:
    //������Ϣ
    virtual BOOL OnInitDialog();
    //�ؼ���
    virtual VOID DoDataExchange(CDataExchange * pDX);
    //�¼���Ϣ
    afx_msg LRESULT OnMessageEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
};
