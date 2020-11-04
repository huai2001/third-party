#include "StdAfx.h"
#include "Resource.h"
#include "SkinEncircle.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CEncircleBMP::CEncircleBMP()
{
	//���ñ���
	m_hResInstance=NULL;
	ZeroMemory(&m_EncircleInfo,sizeof(m_EncircleInfo));
	ZeroMemory(&m_EncircleResource,sizeof(m_EncircleResource));
	m_bRenderImage	= true;

	return;
}

//��������
CEncircleBMP::~CEncircleBMP()
{
}

//������Դ
bool CEncircleBMP::LoadEncircleImage(tagEncircleBMP & EncircleImage)
{
	//������Դ
	if (m_hResInstance==NULL)
	{
		//�ļ�����
		if(m_EncircleResource.pszImageTM)
		{
			EncircleImage.ImageTL.Load(m_EncircleResource.pszImageTL);
			EncircleImage.ImageTM.Load(m_EncircleResource.pszImageTM);
			EncircleImage.ImageTR.Load(m_EncircleResource.pszImageTR);
		}
		EncircleImage.ImageML.Load(m_EncircleResource.pszImageML);
		EncircleImage.ImageMR.Load(m_EncircleResource.pszImageMR);
		EncircleImage.ImageBL.Load(m_EncircleResource.pszImageBL);
		EncircleImage.ImageBM.Load(m_EncircleResource.pszImageBM);
		EncircleImage.ImageBR.Load(m_EncircleResource.pszImageBR);
	}
	else
	{
		//��Դ����
		if(m_EncircleResource.pszImageTM)
		{
			EncircleImage.ImageTL.LoadFromResource(m_hResInstance,m_EncircleResource.pszImageTL);
			EncircleImage.ImageTM.LoadFromResource(m_hResInstance,m_EncircleResource.pszImageTM);
			EncircleImage.ImageTR.LoadFromResource(m_hResInstance,m_EncircleResource.pszImageTR);
		}
		EncircleImage.ImageML.LoadFromResource(m_hResInstance,m_EncircleResource.pszImageML);
		EncircleImage.ImageMR.LoadFromResource(m_hResInstance,m_EncircleResource.pszImageMR);
		EncircleImage.ImageBL.LoadFromResource(m_hResInstance,m_EncircleResource.pszImageBL);
		EncircleImage.ImageBM.LoadFromResource(m_hResInstance,m_EncircleResource.pszImageBM);
		EncircleImage.ImageBR.LoadFromResource(m_hResInstance,m_EncircleResource.pszImageBR);
	}

	return true;
}

//Ĭ����Դ
bool CEncircleBMP::InitDefaultResource()
{
	//��������
	tagEncircleResource EncircleResource;
	ZeroMemory(&EncircleResource,sizeof(EncircleResource));

	//���ñ���
	EncircleResource.pszImageTL=MAKEINTRESOURCE(IDB_ENCIRCLE_TL);
	EncircleResource.pszImageTM=MAKEINTRESOURCE(IDB_ENCIRCLE_TM);
	EncircleResource.pszImageTR=MAKEINTRESOURCE(IDB_ENCIRCLE_TR);
	EncircleResource.pszImageML=MAKEINTRESOURCE(IDB_ENCIRCLE_ML);
	EncircleResource.pszImageMR=MAKEINTRESOURCE(IDB_ENCIRCLE_MR);
	EncircleResource.pszImageBL=MAKEINTRESOURCE(IDB_ENCIRCLE_BL);
	EncircleResource.pszImageBM=MAKEINTRESOURCE(IDB_ENCIRCLE_BM);
	EncircleResource.pszImageBR=MAKEINTRESOURCE(IDB_ENCIRCLE_BR);

	//������Ϣ
	return InitEncircleResource(EncircleResource,GetModuleHandle(SKIN_CONTROLS_DLL_NAME));
}

//������Ϣ
bool CEncircleBMP::InitEncircleResource(tagEncircleResource & EncircleResource, HINSTANCE hResInstance)
{
	//��������
	CImage ImageTM;
	CImage ImageML;
	CImage ImageMR;
	CImage ImageBM;

	//���ñ���
	m_hResInstance=hResInstance;
	m_EncircleResource=EncircleResource;
	memset(&m_EncircleInfo,0,sizeof(m_EncircleInfo));

	//������Դ
	if (hResInstance==NULL)
	{
		//�ļ�����
		if(m_EncircleResource.pszImageTM)ImageTM.Load(m_EncircleResource.pszImageTM);
		ImageML.Load(m_EncircleResource.pszImageML);
		ImageMR.Load(m_EncircleResource.pszImageMR);
		ImageBM.Load(m_EncircleResource.pszImageBM);
	}
	else
	{
		//��Դ����
		if(m_EncircleResource.pszImageTM)ImageTM.LoadFromResource(hResInstance,m_EncircleResource.pszImageTM);
		ImageML.LoadFromResource(hResInstance,m_EncircleResource.pszImageML);
		ImageMR.LoadFromResource(hResInstance,m_EncircleResource.pszImageMR);
		ImageBM.LoadFromResource(hResInstance,m_EncircleResource.pszImageBM);
	}

	//��ȡ��Ϣ
	m_EncircleInfo.nLBorder=ImageML.GetWidth();
	m_EncircleInfo.nRBorder=ImageMR.GetWidth();
	if(m_EncircleResource.pszImageTM)m_EncircleInfo.nTBorder=ImageTM.GetHeight();
	m_EncircleInfo.nBBorder=ImageBM.GetHeight();

	return true;
}

//�滭����
bool CEncircleBMP::DrawEncircleFrame(CDC * pDC, CRect & rcEncircle)
{
	//��������
	INT nXTerminate=0;
	INT nYTerminate=0;
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//������Դ
	tagEncircleBMP EncircleImage;
	LoadEncircleImage(EncircleImage);

	//��Ⱦ��Դ
	if(m_bRenderImage)
	{
		if(m_EncircleResource.pszImageTM)
		{
			pSkinRenderManager->RenderImage(EncircleImage.ImageTL);
			pSkinRenderManager->RenderImage(EncircleImage.ImageTM);
			pSkinRenderManager->RenderImage(EncircleImage.ImageTR);
		}
		pSkinRenderManager->RenderImage(EncircleImage.ImageML);
		pSkinRenderManager->RenderImage(EncircleImage.ImageMR);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBL);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBM);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBR);
	}

	//��Դ����
	PreDrawEncircleImage(EncircleImage);

	//��ȡλ��
	CSize SizeImageTL,SizeImageTR,SizeImageTM,SizeImageBL,SizeImageBR;
	if(m_EncircleResource.pszImageTM)
	{
		SizeImageTL.SetSize(EncircleImage.ImageTL.GetWidth(),EncircleImage.ImageTL.GetHeight());
		SizeImageTR.SetSize(EncircleImage.ImageTR.GetWidth(),EncircleImage.ImageTR.GetHeight());
		SizeImageTM.SetSize(EncircleImage.ImageTM.GetWidth(),EncircleImage.ImageTM.GetHeight());
	}
	else
	{
		SizeImageTL.SetSize(0,0);
		SizeImageTM.SetSize(0,0);
		SizeImageTR.SetSize(0,0);
	}
	SizeImageBL.SetSize(EncircleImage.ImageBL.GetWidth(),EncircleImage.ImageBL.GetHeight());
	SizeImageBR.SetSize(EncircleImage.ImageBR.GetWidth(),EncircleImage.ImageBR.GetHeight());

	//��ȡλ��
	CSize SizeImageMR,SizeImageML,SizeImageBM;
	SizeImageMR.SetSize(EncircleImage.ImageMR.GetWidth(),EncircleImage.ImageMR.GetHeight());
	SizeImageML.SetSize(EncircleImage.ImageML.GetWidth(),EncircleImage.ImageML.GetHeight());
	SizeImageBM.SetSize(EncircleImage.ImageBM.GetWidth(),EncircleImage.ImageBM.GetHeight());

	//�滭�ϱ�
	if(m_EncircleResource.pszImageTM)
	{
		nXTerminate=rcEncircle.right-SizeImageTR.cx;
		for (INT nXPos=rcEncircle.left+SizeImageTL.cx;nXPos<nXTerminate;nXPos+=SizeImageTM.cx)
		{
			EncircleImage.ImageTM.BitBlt(pDC->m_hDC,nXPos,rcEncircle.top,__min(nXTerminate-nXPos,SizeImageTM.cx),SizeImageTM.cy,0,0);
		}
	}
	
	//�滭�±�
	nXTerminate=rcEncircle.right-SizeImageBR.cx;
	for (INT nXPos=rcEncircle.left+SizeImageBL.cx;nXPos<nXTerminate;nXPos+=SizeImageBM.cx)
	{
		EncircleImage.ImageBM.BitBlt(pDC->m_hDC,nXPos,rcEncircle.bottom-SizeImageBM.cy,__min(nXTerminate-nXPos,SizeImageBM.cx),SizeImageBM.cy,0,0);
	}

	//�滭���
	nYTerminate=rcEncircle.bottom-SizeImageBL.cy;
	for (INT nYPos=rcEncircle.top+SizeImageTL.cy;nYPos<nYTerminate;nYPos+=SizeImageML.cy)
	{
		EncircleImage.ImageML.BitBlt(pDC->m_hDC,rcEncircle.left,nYPos,SizeImageML.cx,__min(nYTerminate-nYPos,SizeImageML.cy),0,0);
	}

	//�滭�ұ�
	nYTerminate=rcEncircle.bottom-SizeImageBR.cy;
	for (INT nYPos=rcEncircle.top+SizeImageTR.cy;nYPos<nYTerminate;nYPos+=SizeImageMR.cy)
	{
		EncircleImage.ImageMR.BitBlt(pDC->m_hDC,rcEncircle.right-SizeImageMR.cx,nYPos,SizeImageMR.cx,__min(nYTerminate-nYPos,SizeImageMR.cy),0,0);
	}

	//�滭�Ľ�
	if(m_EncircleResource.pszImageTM)
	{
		EncircleImage.ImageTL.BitBlt(pDC->m_hDC,rcEncircle.left,rcEncircle.top);
		EncircleImage.ImageTR.BitBlt(pDC->m_hDC,rcEncircle.right-SizeImageTR.cx,rcEncircle.top);
	}
	EncircleImage.ImageBL.BitBlt(pDC->m_hDC,rcEncircle.left,rcEncircle.bottom-SizeImageBL.cy);
	EncircleImage.ImageBR.BitBlt(pDC->m_hDC,rcEncircle.right-SizeImageBR.cx,rcEncircle.bottom-SizeImageBR.cy);

	return true;
}

//�滭����
bool CEncircleBMP::DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, COLORREF crTrans)
{
	//��������
	INT nXTerminate=0;
	INT nYTerminate=0;
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//������Դ
	tagEncircleBMP EncircleImage;
	LoadEncircleImage(EncircleImage);

	//��Ⱦ��Դ
	if(m_bRenderImage)
	{
		pSkinRenderManager->RenderImage(EncircleImage.ImageTL);
		pSkinRenderManager->RenderImage(EncircleImage.ImageTM);
		pSkinRenderManager->RenderImage(EncircleImage.ImageTR);
		pSkinRenderManager->RenderImage(EncircleImage.ImageML);
		pSkinRenderManager->RenderImage(EncircleImage.ImageMR);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBL);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBM);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBR);
	}
	
	//��Դ����
	PreDrawEncircleImage(EncircleImage);

	//��ȡλ��
	CSize SizeImageTL,SizeImageTR,SizeImageBL,SizeImageBR;
	SizeImageTL.SetSize(EncircleImage.ImageTL.GetWidth(),EncircleImage.ImageTL.GetHeight());
	SizeImageTR.SetSize(EncircleImage.ImageTR.GetWidth(),EncircleImage.ImageTR.GetHeight());
	SizeImageBL.SetSize(EncircleImage.ImageBL.GetWidth(),EncircleImage.ImageBL.GetHeight());
	SizeImageBR.SetSize(EncircleImage.ImageBR.GetWidth(),EncircleImage.ImageBR.GetHeight());

	//��ȡλ��
	CSize SizeImageMR,SizeImageML,SizeImageTM,SizeImageBM;
	SizeImageMR.SetSize(EncircleImage.ImageMR.GetWidth(),EncircleImage.ImageMR.GetHeight());
	SizeImageML.SetSize(EncircleImage.ImageML.GetWidth(),EncircleImage.ImageML.GetHeight());
	SizeImageTM.SetSize(EncircleImage.ImageTM.GetWidth(),EncircleImage.ImageTM.GetHeight());
	SizeImageBM.SetSize(EncircleImage.ImageBM.GetWidth(),EncircleImage.ImageBM.GetHeight());

	//�滭�ϱ�
	nXTerminate=rcEncircle.right-SizeImageTR.cx;
	for (INT nXPos=rcEncircle.left+SizeImageTL.cx;nXPos<nXTerminate;nXPos+=SizeImageTM.cx)
	{
		EncircleImage.ImageTM.TransDrawImage(pDC,nXPos,rcEncircle.top,__min(nXTerminate-nXPos,SizeImageTM.cx),SizeImageTM.cy,0,0,crTrans);
	}

	//�滭�±�
	nXTerminate=rcEncircle.right-SizeImageBR.cx;
	for (INT nXPos=rcEncircle.left+SizeImageBL.cx;nXPos<nXTerminate;nXPos+=SizeImageBM.cx)
	{
		EncircleImage.ImageBM.TransDrawImage(pDC,nXPos,rcEncircle.bottom-SizeImageBM.cy,__min(nXTerminate-nXPos,SizeImageBM.cx),SizeImageBM.cy,0,0,crTrans);
	}

	//�滭���
	nYTerminate=rcEncircle.bottom-SizeImageBL.cy;
	for (INT nYPos=rcEncircle.top+SizeImageTL.cy;nYPos<nYTerminate;nYPos+=SizeImageML.cy)
	{
		EncircleImage.ImageML.TransDrawImage(pDC,rcEncircle.left,nYPos,SizeImageML.cx,__min(nYTerminate-nYPos,SizeImageML.cy),0,0,crTrans);
	}

	//�滭�ұ�
	nYTerminate=rcEncircle.bottom-SizeImageBR.cy;
	for (INT nYPos=rcEncircle.top+SizeImageTR.cy;nYPos<nYTerminate;nYPos+=SizeImageMR.cy)
	{
		EncircleImage.ImageMR.TransDrawImage(pDC,rcEncircle.right-SizeImageMR.cx,nYPos,SizeImageMR.cx,__min(nYTerminate-nYPos,SizeImageMR.cy),0,0,crTrans);
	}

	//�滭�Ľ�
	EncircleImage.ImageTL.TransDrawImage(pDC,rcEncircle.left,rcEncircle.top,crTrans);
	EncircleImage.ImageTR.TransDrawImage(pDC,rcEncircle.right-SizeImageTR.cx,rcEncircle.top,crTrans);
	EncircleImage.ImageBL.TransDrawImage(pDC,rcEncircle.left,rcEncircle.bottom-SizeImageBL.cy,crTrans);
	EncircleImage.ImageBR.TransDrawImage(pDC,rcEncircle.right-SizeImageBR.cx,rcEncircle.bottom-SizeImageBR.cy,crTrans);

	return true;
}

//�滭����
bool CEncircleBMP::DrawEncircleFrame(CDC * pDC, CRect & rcEncircle, COLORREF crTrans, BYTE cbAlphaDepth)
{
	//��������
	INT nXTerminate=0;
	INT nYTerminate=0;
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

	//������Դ
	tagEncircleBMP EncircleImage;
	LoadEncircleImage(EncircleImage);
	
	//��Ⱦ��Դ
	if(m_bRenderImage)
	{
		pSkinRenderManager->RenderImage(EncircleImage.ImageTL);
		pSkinRenderManager->RenderImage(EncircleImage.ImageTM);
		pSkinRenderManager->RenderImage(EncircleImage.ImageTR);
		pSkinRenderManager->RenderImage(EncircleImage.ImageML);
		pSkinRenderManager->RenderImage(EncircleImage.ImageMR);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBL);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBM);
		pSkinRenderManager->RenderImage(EncircleImage.ImageBR);
	}

	//��Դ����
	PreDrawEncircleImage(EncircleImage);

	//��ȡλ��
	CSize SizeImageTL,SizeImageTR,SizeImageBL,SizeImageBR;
	SizeImageTL.SetSize(EncircleImage.ImageTL.GetWidth(),EncircleImage.ImageTL.GetHeight());
	SizeImageTR.SetSize(EncircleImage.ImageTR.GetWidth(),EncircleImage.ImageTR.GetHeight());
	SizeImageBL.SetSize(EncircleImage.ImageBL.GetWidth(),EncircleImage.ImageBL.GetHeight());
	SizeImageBR.SetSize(EncircleImage.ImageBR.GetWidth(),EncircleImage.ImageBR.GetHeight());

	//��ȡλ��
	CSize SizeImageMR,SizeImageML,SizeImageTM,SizeImageBM;
	SizeImageMR.SetSize(EncircleImage.ImageMR.GetWidth(),EncircleImage.ImageMR.GetHeight());
	SizeImageML.SetSize(EncircleImage.ImageML.GetWidth(),EncircleImage.ImageML.GetHeight());
	SizeImageTM.SetSize(EncircleImage.ImageTM.GetWidth(),EncircleImage.ImageTM.GetHeight());
	SizeImageBM.SetSize(EncircleImage.ImageBM.GetWidth(),EncircleImage.ImageBM.GetHeight());

	//�滭�ϱ�
	nXTerminate=rcEncircle.right-SizeImageTR.cx;
	for (INT nXPos=rcEncircle.left+SizeImageTL.cx;nXPos<nXTerminate;nXPos+=SizeImageTM.cx)
	{
		EncircleImage.ImageTM.BlendDrawImage(pDC,nXPos,rcEncircle.top,__min(nXTerminate-nXPos,SizeImageTM.cx),SizeImageTM.cy,0,0,crTrans,cbAlphaDepth);
	}

	//�滭�±�
	nXTerminate=rcEncircle.right-SizeImageBR.cx;
	for (INT nXPos=rcEncircle.left+SizeImageBL.cx;nXPos<nXTerminate;nXPos+=SizeImageBM.cx)
	{
		EncircleImage.ImageBM.BlendDrawImage(pDC,nXPos,rcEncircle.bottom-SizeImageBM.cy,__min(nXTerminate-nXPos,SizeImageBM.cx),SizeImageBM.cy,0,0,crTrans,cbAlphaDepth);
	}

	//�滭���
	nYTerminate=rcEncircle.bottom-SizeImageBL.cy;
	for (INT nYPos=rcEncircle.top+SizeImageTL.cy;nYPos<nYTerminate;nYPos+=SizeImageML.cy)
	{
		EncircleImage.ImageML.BlendDrawImage(pDC,rcEncircle.left,nYPos,SizeImageML.cx,__min(nYTerminate-nYPos,SizeImageML.cy),0,0,crTrans,cbAlphaDepth);
	}

	//�滭�ұ�
	nYTerminate=rcEncircle.bottom-SizeImageBR.cy;
	for (INT nYPos=rcEncircle.top+SizeImageTR.cy;nYPos<nYTerminate;nYPos+=SizeImageMR.cy)
	{
		EncircleImage.ImageMR.BlendDrawImage(pDC,rcEncircle.right-SizeImageMR.cx,nYPos,SizeImageMR.cx,__min(nYTerminate-nYPos,SizeImageMR.cy),0,0,crTrans,cbAlphaDepth);
	}

	//�滭�Ľ�
	EncircleImage.ImageTL.BlendDrawImage(pDC,rcEncircle.left,rcEncircle.top,crTrans,cbAlphaDepth);
	EncircleImage.ImageTR.BlendDrawImage(pDC,rcEncircle.right-SizeImageTR.cx,rcEncircle.top,crTrans,cbAlphaDepth);
	EncircleImage.ImageBL.BlendDrawImage(pDC,rcEncircle.left,rcEncircle.bottom-SizeImageBL.cy,crTrans,cbAlphaDepth);
	EncircleImage.ImageBR.BlendDrawImage(pDC,rcEncircle.right-SizeImageBR.cx,rcEncircle.bottom-SizeImageBR.cy,crTrans,cbAlphaDepth);

	return true;
}

//��ȡλ��
bool CEncircleBMP::GetEncircleInfo(tagEncircleInfo & EncircleInfo)
{
	//���ñ���
	EncircleInfo=m_EncircleInfo;

	return true;
}

//��ȡλ��
bool CEncircleBMP::GetEncircleRect(CRect & rcEncircleBorad, CRect & rcControl)
{
	//����λ��
	rcControl.top=rcEncircleBorad.top+m_EncircleInfo.nTBorder;
	rcControl.left=rcEncircleBorad.left+m_EncircleInfo.nLBorder;
	rcControl.right=rcEncircleBorad.right-m_EncircleInfo.nRBorder;
	rcControl.bottom=rcEncircleBorad.bottom-m_EncircleInfo.nBBorder;

	return true;
}

//���Ƶ���
bool CEncircleBMP::SetWindowPos(CWnd * pWnd, CRect & rcEncircleBorad)
{
	//��������
	if (pWnd->GetSafeHwnd()!=NULL)
	{
		//����λ��
		CRect rcControl;
		GetEncircleRect(rcEncircleBorad,rcControl);

		//��������
		UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;
		pWnd->SetWindowPos(NULL,rcControl.left,rcControl.top,rcControl.Width(),rcControl.Height(),uFlags);

		return true;
	}

	return false;
}

//���Ƶ���
bool CEncircleBMP::DeferWindowPos(CWnd * pWnd, HDWP hDwp, CRect & rcEncircleBorad)
{
	//��������
	if (pWnd->GetSafeHwnd()!=NULL)
	{
		//����λ��
		CRect rcControl;
		GetEncircleRect(rcEncircleBorad,rcControl);

		//��������
		UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;
		::DeferWindowPos(hDwp,pWnd->m_hWnd,NULL,rcControl.left,rcControl.top,rcControl.Width(),rcControl.Height(),uFlags);

		return true;
	}

	return false;
}

void CEncircleBMP::SetRenderImage( bool bReander )
{
	m_bRenderImage = bReander;
}
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CEncirclePNG::CEncirclePNG()
{
	//���ñ���
	m_hResInstance=NULL;
	ZeroMemory(&m_EncircleInfo,sizeof(m_EncircleInfo));
	ZeroMemory(&m_EncircleResource,sizeof(m_EncircleResource));

	return;
}

//��������
CEncirclePNG::~CEncirclePNG()
{
}

//������Դ
bool CEncirclePNG::LoadEncircleImage(tagEncirclePNG & EncircleImage)
{
	//������Դ
	if (m_hResInstance==NULL)
	{
		//�ļ�����
		EncircleImage.ImageTL.LoadImage(m_EncircleResource.pszImageTL);
		EncircleImage.ImageTM.LoadImage(m_EncircleResource.pszImageTM);
		EncircleImage.ImageTR.LoadImage(m_EncircleResource.pszImageTR);
		EncircleImage.ImageML.LoadImage(m_EncircleResource.pszImageML);
		EncircleImage.ImageMR.LoadImage(m_EncircleResource.pszImageMR);
		EncircleImage.ImageBL.LoadImage(m_EncircleResource.pszImageBL);
		EncircleImage.ImageBM.LoadImage(m_EncircleResource.pszImageBM);
		EncircleImage.ImageBR.LoadImage(m_EncircleResource.pszImageBR);
	}
	else
	{
		//��Դ����
		EncircleImage.ImageTL.LoadImage(m_hResInstance,m_EncircleResource.pszImageTL);
		EncircleImage.ImageTM.LoadImage(m_hResInstance,m_EncircleResource.pszImageTM);
		EncircleImage.ImageTR.LoadImage(m_hResInstance,m_EncircleResource.pszImageTR);
		EncircleImage.ImageML.LoadImage(m_hResInstance,m_EncircleResource.pszImageML);
		EncircleImage.ImageMR.LoadImage(m_hResInstance,m_EncircleResource.pszImageMR);
		EncircleImage.ImageBL.LoadImage(m_hResInstance,m_EncircleResource.pszImageBL);
		EncircleImage.ImageBM.LoadImage(m_hResInstance,m_EncircleResource.pszImageBM);
		EncircleImage.ImageBR.LoadImage(m_hResInstance,m_EncircleResource.pszImageBR);
	}

	return true;
}

//Ĭ����Դ
bool CEncirclePNG::InitDefaultResource()
{
	//��������
	tagEncircleResource EncircleResource;
	ZeroMemory(&EncircleResource,sizeof(EncircleResource));

	//���ñ���
	EncircleResource.pszImageTL=MAKEINTRESOURCE(IDB_ENCIRCLE_TL);
	EncircleResource.pszImageTM=MAKEINTRESOURCE(IDB_ENCIRCLE_TM);
	EncircleResource.pszImageTR=MAKEINTRESOURCE(IDB_ENCIRCLE_TR);
	EncircleResource.pszImageML=MAKEINTRESOURCE(IDB_ENCIRCLE_ML);
	EncircleResource.pszImageMR=MAKEINTRESOURCE(IDB_ENCIRCLE_MR);
	EncircleResource.pszImageBL=MAKEINTRESOURCE(IDB_ENCIRCLE_BL);
	EncircleResource.pszImageBM=MAKEINTRESOURCE(IDB_ENCIRCLE_BM);
	EncircleResource.pszImageBR=MAKEINTRESOURCE(IDB_ENCIRCLE_BR);

	//������Ϣ
	return InitEncircleResource(EncircleResource,GetModuleHandle(SKIN_CONTROLS_DLL_NAME));
}

//������Ϣ
bool CEncirclePNG::InitEncircleResource(tagEncircleResource & EncircleResource, HINSTANCE hResInstance)
{
	//��������
	CPNGImage ImageTM;
	CPNGImage ImageML;
	CPNGImage ImageMR;
	CPNGImage ImageBM;

	//���ñ���
	m_hResInstance=hResInstance;
	m_EncircleResource=EncircleResource;

	//������Դ
	if (hResInstance==NULL)
	{
		//�ļ�����
		ImageTM.LoadImage(m_EncircleResource.pszImageTM);
		ImageML.LoadImage(m_EncircleResource.pszImageML);
		ImageMR.LoadImage(m_EncircleResource.pszImageMR);
		ImageBM.LoadImage(m_EncircleResource.pszImageBM);
	}
	else
	{
		//��Դ����
		ImageTM.LoadImage(hResInstance,m_EncircleResource.pszImageTM);
		ImageML.LoadImage(hResInstance,m_EncircleResource.pszImageML);
		ImageMR.LoadImage(hResInstance,m_EncircleResource.pszImageMR);
		ImageBM.LoadImage(hResInstance,m_EncircleResource.pszImageBM);
	}

	//Ч����Դ
	ASSERT(ImageTM.IsNull()==false);
	ASSERT(ImageML.IsNull()==false);
	ASSERT(ImageMR.IsNull()==false);
	ASSERT(ImageBM.IsNull()==false);

	//��ȡ��Ϣ
	m_EncircleInfo.nLBorder=ImageML.GetWidth();
	m_EncircleInfo.nRBorder=ImageMR.GetWidth();
	m_EncircleInfo.nTBorder=ImageTM.GetHeight();
	m_EncircleInfo.nBBorder=ImageBM.GetHeight();

	return true;
}

//�滭����
bool CEncirclePNG::DrawEncircleFrame(CDC * pDC, CRect & rcEncircle)
{
	//��������
	INT nXTerminate=0;
	INT nYTerminate=0;

	//������Դ
	tagEncirclePNG EncircleImage;
	LoadEncircleImage(EncircleImage);

	//��Դ����
	PreDrawEncircleImage(EncircleImage);

	//��ȡλ��
	CSize SizeImageTL,SizeImageTR,SizeImageBL,SizeImageBR;
	SizeImageTL.SetSize(EncircleImage.ImageTL.GetWidth(),EncircleImage.ImageTL.GetHeight());
	SizeImageTR.SetSize(EncircleImage.ImageTR.GetWidth(),EncircleImage.ImageTR.GetHeight());
	SizeImageBL.SetSize(EncircleImage.ImageBL.GetWidth(),EncircleImage.ImageBL.GetHeight());
	SizeImageBR.SetSize(EncircleImage.ImageBR.GetWidth(),EncircleImage.ImageBR.GetHeight());

	//��ȡλ��
	CSize SizeImageMR,SizeImageML,SizeImageTM,SizeImageBM;
	SizeImageMR.SetSize(EncircleImage.ImageMR.GetWidth(),EncircleImage.ImageMR.GetHeight());
	SizeImageML.SetSize(EncircleImage.ImageML.GetWidth(),EncircleImage.ImageML.GetHeight());
	SizeImageTM.SetSize(EncircleImage.ImageTM.GetWidth(),EncircleImage.ImageTM.GetHeight());
	SizeImageBM.SetSize(EncircleImage.ImageBM.GetWidth(),EncircleImage.ImageBM.GetHeight());

	//�滭�ϱ�
	nXTerminate=rcEncircle.right-SizeImageTR.cx;
	for (INT nXPos=rcEncircle.left+SizeImageTL.cx;nXPos<nXTerminate;nXPos+=SizeImageTM.cx)
	{
		EncircleImage.ImageTM.DrawImage(pDC,nXPos,rcEncircle.top,__min(nXTerminate-nXPos,SizeImageTM.cx),SizeImageTM.cy,0,0);
	}

	//�滭�±�
	nXTerminate=rcEncircle.right-SizeImageBR.cx;
	for (INT nXPos=rcEncircle.left+SizeImageBL.cx;nXPos<nXTerminate;nXPos+=SizeImageBM.cx)
	{
		EncircleImage.ImageBM.DrawImage(pDC,nXPos,rcEncircle.bottom-SizeImageBM.cy,__min(nXTerminate-nXPos,SizeImageBM.cx),SizeImageBM.cy,0,0);
	}

	//�滭���
	nYTerminate=rcEncircle.bottom-SizeImageBL.cy;
	for (INT nYPos=rcEncircle.top+SizeImageTL.cy;nYPos<nYTerminate;nYPos+=SizeImageML.cy)
	{
		EncircleImage.ImageML.DrawImage(pDC,rcEncircle.left,nYPos,SizeImageML.cx,__min(nYTerminate-nYPos,SizeImageML.cy),0,0);
	}

	//�滭�ұ�
	nYTerminate=rcEncircle.bottom-SizeImageBR.cy;
	for (INT nYPos=rcEncircle.top+SizeImageTR.cy;nYPos<nYTerminate;nYPos+=SizeImageMR.cy)
	{
		EncircleImage.ImageMR.DrawImage(pDC,rcEncircle.right-SizeImageMR.cx,nYPos,SizeImageMR.cx,__min(nYTerminate-nYPos,SizeImageMR.cy),0,0);
	}

	//�滭�Ľ�
	EncircleImage.ImageTL.DrawImage(pDC,rcEncircle.left,rcEncircle.top);
	EncircleImage.ImageTR.DrawImage(pDC,rcEncircle.right-SizeImageTR.cx,rcEncircle.top);
	EncircleImage.ImageBL.DrawImage(pDC,rcEncircle.left,rcEncircle.bottom-SizeImageBL.cy);
	EncircleImage.ImageBR.DrawImage(pDC,rcEncircle.right-SizeImageBR.cx,rcEncircle.bottom-SizeImageBR.cy);

	return true;
}

//��ȡλ��
bool CEncirclePNG::GetEncircleInfo(tagEncircleInfo & EncircleInfo)
{
	//���ñ���
	EncircleInfo=m_EncircleInfo;

	return true;
}

//��ȡλ��
bool CEncirclePNG::GetEncircleRect(CRect & rcEncircleBorad, CRect & rcControl)
{
	//����λ��
	rcControl.top=rcEncircleBorad.top+m_EncircleInfo.nTBorder;
	rcControl.left=rcEncircleBorad.left+m_EncircleInfo.nLBorder;
	rcControl.right=rcEncircleBorad.right-m_EncircleInfo.nRBorder;
	rcControl.bottom=rcEncircleBorad.bottom-m_EncircleInfo.nBBorder;

	return true;
}

//���Ƶ���
bool CEncirclePNG::SetWindowPos(CWnd * pWnd, CRect & rcEncircleBorad)
{
	//��������
	if (pWnd->GetSafeHwnd()!=NULL)
	{
		//����λ��
		CRect rcControl;
		GetEncircleRect(rcEncircleBorad,rcControl);

		//��������
		UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;
		pWnd->SetWindowPos(NULL,rcControl.left,rcControl.top,rcControl.Width(),rcControl.Height(),uFlags);

		return true;
	}

	return false;
}

//���Ƶ���
bool CEncirclePNG::DeferWindowPos(CWnd * pWnd, HDWP hDwp, CRect & rcEncircleBorad)
{
	//��������
	if (pWnd->GetSafeHwnd()!=NULL)
	{
		//����λ��
		CRect rcControl;
		GetEncircleRect(rcEncircleBorad,rcControl);

		//��������
		UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;
		::DeferWindowPos(hDwp,pWnd->m_hWnd,NULL,rcControl.left,rcControl.top,rcControl.Width(),rcControl.Height(),uFlags);

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////
