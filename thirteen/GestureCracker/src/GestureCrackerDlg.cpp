
// GestureCrackerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GestureCracker.h"
#include "GestureCrackerDlg.h"
#include "afxdialogex.h"

#include "Cracker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGestureCrackerDlg �Ի���




CGestureCrackerDlg::CGestureCrackerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGestureCrackerDlg::IDD, pParent)
	, m_strFileName(_T(""))
	, m_nGestureLength(-1)
{
	ZeroMemory(m_aGesture, sizeof(m_aGesture));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGestureCrackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_FILE, m_strFileName);
}

BEGIN_MESSAGE_MAP(CGestureCrackerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD, &CGestureCrackerDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_CRACK, &CGestureCrackerDlg::OnBnClickedBtnCrack)
	ON_BN_CLICKED(IDC_BTN_REDRAW, &CGestureCrackerDlg::OnBnClickedBtnReDraw)
END_MESSAGE_MAP()


// CGestureCrackerDlg ��Ϣ�������

BOOL CGestureCrackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGestureCrackerDlg::OnPaint()
{

	DrawGesture();

	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGestureCrackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGestureCrackerDlg::OnBnClickedBtnLoad()
{
	CFileDialog cf(
		TRUE,
		_T("key"),
		_T("gesture"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("KeyFile(*.key)|*.key;||"),
		this
		);

	cf.DoModal();
	m_strFileName = cf.GetPathName();
	
	UpdateData(FALSE);
}

void CGestureCrackerDlg::OnBnClickedBtnCrack()
{
	CCracker ck;
	CString strEncode;
	if (m_strFileName.IsEmpty())
	{
		MessageBox(_T("���ȴ�Key�ļ���"));
		return;
	}

	CFile cf(m_strFileName, CFile::modeRead);
	if (cf.GetLength()!=20)
	{
		MessageBox(_T("�ļ�����"));
		return;
	}

	CString strTemp;
	int cTemp = 0;
	for (int i=0;i<20;i++)
	{
		cf.Seek(i, CFile::begin);
		cf.Read(&cTemp, 1);
		//ע�������ǽ�ʮ��������ת����Сд��ʽ���ʺ��治�ÿ����ַ����Ƚ�����
		strTemp.Format(_T("%.2x"), cTemp);
		strEncode+=strTemp;
	}
	
	if (ck.Crack(strEncode.GetString()))
	{
		m_nGestureLength = ck.GetGestureLength();
		memcpy(m_aGesture, ck.GetGesture(), m_nGestureLength*sizeof(&m_aGesture));
		MessageBox(_T("�㶨�չ���"));
		OnPaint();
		return;
	}
	else
	{
		MessageBox(_T("������ 0x002"));
		return;
	}
}


void CGestureCrackerDlg::DrawGesture(void)
{
	CClientDC cdc(GetDlgItem(IDC_STC_CANVAS));
	CRect CanvasRect;
	cdc.GetWindow()->GetWindowRect(CanvasRect);

	int basic_x = CanvasRect.Width()/2;
	int basic_y = CanvasRect.Height()/2;

	
	CPoint aPoint[9];
	for (int iy=0;iy<3;iy++)
	{
		for (int ix=0;ix<3;ix++)
		{
			aPoint[iy*3+ix].SetPoint(ix*basic_x, iy*basic_y);
		}
	}

	const int RADIUS = 20;
	for (int i=0;i<9;i++)
	{
		CRect r;
		r.top = aPoint[i].y - RADIUS;
		r.bottom = aPoint[i].y + RADIUS;
		r.left = aPoint[i].x - RADIUS;
		r.right = aPoint[i].x + RADIUS;
		cdc.Arc(r, aPoint[i], aPoint[i]);
	}

	CPen tPen;
	tPen.CreatePen(PS_SOLID, 3, RGB(0,0,255));
	if (m_nGestureLength>0)
	{
		cdc.MoveTo(aPoint[m_aGesture[0]]);
		cdc.TextOut(aPoint[m_aGesture[0]].x, aPoint[m_aGesture[0]].y, _T("���"));
		cdc.SelectObject(tPen);
		for (int i=0;i<m_nGestureLength;i++)
			cdc.LineTo(aPoint[m_aGesture[i]]);
	}
}


void CGestureCrackerDlg::OnBnClickedBtnReDraw()
{
	OnPaint();
}
