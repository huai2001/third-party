// SettingDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Crawler.h"
#include "SettingDialog.h"
#include "afxdialogex.h"


// CSettingDialog �Ի���

IMPLEMENT_DYNAMIC(CSettingDialog, CSkinDialog)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CSettingDialog::IDD, pParent)
{
    
    for (int i = 0; i < _cc_countof(m_StaticColor); i++) {
        m_StaticColor[i].SetIndex(i);
    }

    m_hFromWnd = NULL;
}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
    __super::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_EDIT_POS_X1, m_EditPosX[0]);
    DDX_Control(pDX, IDC_EDIT_POS_Y1, m_EditPosY[0]);
    DDX_Control(pDX, IDC_EDIT_COLOR_1, m_EditColor[0]);


    DDX_Control(pDX, IDC_EDIT_POS_X2, m_EditPosX[1]);
    DDX_Control(pDX, IDC_EDIT_POS_Y2, m_EditPosY[1]);
    DDX_Control(pDX, IDC_EDIT_COLOR_2, m_EditColor[1]);

    DDX_Control(pDX, IDC_EDIT_POS_X3, m_EditPosX[2]);
    DDX_Control(pDX, IDC_EDIT_POS_Y3, m_EditPosY[2]);
    DDX_Control(pDX, IDC_EDIT_COLOR_3, m_EditColor[2]);


    DDX_Control(pDX, IDC_EDIT_POS_X4, m_EditPosX[3]);
    DDX_Control(pDX, IDC_EDIT_POS_Y4, m_EditPosY[3]);
    DDX_Control(pDX, IDC_EDIT_COLOR_4, m_EditColor[3]);

    DDX_Control(pDX, IDC_EDIT_POS_X5, m_EditPosX[4]);
    DDX_Control(pDX, IDC_EDIT_POS_Y5, m_EditPosY[4]);
    DDX_Control(pDX, IDC_EDIT_COLOR_5, m_EditColor[4]);

    DDX_Control(pDX, IDC_EDIT_POS_X6, m_EditPosX[5]);
    DDX_Control(pDX, IDC_EDIT_POS_Y6, m_EditPosY[5]);
    DDX_Control(pDX, IDC_EDIT_COLOR_6, m_EditColor[5]);

    DDX_Control(pDX, IDC_EDIT_POS_X7, m_EditPosX[6]);
    DDX_Control(pDX, IDC_EDIT_POS_Y7, m_EditPosY[6]);
    DDX_Control(pDX, IDC_EDIT_COLOR_7, m_EditColor[6]);

    DDX_Control(pDX, IDC_STATIC_COLOR_1, m_StaticColor[0]);
    DDX_Control(pDX, IDC_STATIC_COLOR_2, m_StaticColor[1]);
    DDX_Control(pDX, IDC_STATIC_COLOR_3, m_StaticColor[2]);
    DDX_Control(pDX, IDC_STATIC_COLOR_4, m_StaticColor[3]);
    DDX_Control(pDX, IDC_STATIC_COLOR_5, m_StaticColor[4]);
    DDX_Control(pDX, IDC_STATIC_COLOR_6, m_StaticColor[5]);
    DDX_Control(pDX, IDC_STATIC_COLOR_7, m_StaticColor[6]);

    DDX_Control(pDX, IDOK, m_btOK);
    DDX_Control(pDX, IDCANCEL, m_btCancal);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CSkinDialog)
    ON_MESSAGE(WM_MESSAGE_EVENT, OnMessageEvent)
    ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingDialog ��Ϣ�������
// CProcessDialog ��Ϣ�������
BOOL CSettingDialog::OnInitDialog()
{
    __super::OnInitDialog();

    HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    // ִ�д˲���
    SetIcon(hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(hIcon, FALSE);			// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    CString buf;
    CPoint mousePoint;
    for (int i = 0; i < 7; i++) {
        mousePoint = gCrawlerConfig.mousePoint[i];
        buf.Format(_T("%6X"), gCrawlerConfig.colors[i]);
        m_EditColor[i].SetWindowText(buf);

        buf.Format(_T("%d"), mousePoint.x);
        m_EditPosX[i].SetWindowText(buf);
        buf.Format(_T("%d"), mousePoint.y);
        m_EditPosY[i].SetWindowText(buf);
        m_StaticColor[i].SetBackColor(gCrawlerConfig.colors[i]);
    }

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


//�¼���Ϣ
LRESULT CSettingDialog::OnMessageEvent(WPARAM wParam, LPARAM lParam)
{
    //�¼�����
    switch (wParam)
    {
    case EVENT_DRAG_STARTED:			//��ȡ�����¼�
        {

        }
        break;
    case EVENT_DRAG_ENDED: {
            CStaticColor *pColor = (CStaticColor*)lParam;
            if (pColor && IsWindow(m_hFromWnd)) {
                CString buf;
                CPoint mousePoint(pColor->m_MousePoint);
                ::ScreenToClient( m_hFromWnd, &mousePoint );
                COLORREF color = pColor->m_BackColor;
                int i = pColor->m_Index;
                buf.Format(_T("%6X"), color);
                m_EditColor[i].SetWindowText(buf);

                buf.Format(_T("%d"), mousePoint.x);
                m_EditPosX[i].SetWindowText(buf);
                buf.Format(_T("%d"), mousePoint.y);
                m_EditPosY[i].SetWindowText(buf);

                gCrawlerConfig.mousePoint[i] = mousePoint;
                gCrawlerConfig.colors[i] = color;
            }
        }
        break;
    }

    return 0L; 
}


void CSettingDialog::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CRect rc;
    ::GetWindowRect(m_hFromWnd, &rc);

    gCrawlerConfig.nWidth = rc.Width();
    gCrawlerConfig.nHeight = rc.Height();
    SaveConfigFile();
    __super::OnOK();
}
