
// GestureCrackerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GestureCracker.h"
#include "GestureCrackerDlg.h"
#include "afxdialogex.h"

#include "Cracker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGestureCrackerDlg 对话框




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


// CGestureCrackerDlg 消息处理程序

BOOL CGestureCrackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGestureCrackerDlg::OnPaint()
{

	DrawGesture();

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		MessageBox(_T("请先打开Key文件！"));
		return;
	}

	CFile cf(m_strFileName, CFile::modeRead);
	if (cf.GetLength()!=20)
	{
		MessageBox(_T("文件错误！"));
		return;
	}

	CString strTemp;
	int cTemp = 0;
	for (int i=0;i<20;i++)
	{
		cf.Seek(i, CFile::begin);
		cf.Read(&cTemp, 1);
		//注意这里是将十六进制数转换成小写形式，故后面不用考虑字符串比较问题
		strTemp.Format(_T("%.2x"), cTemp);
		strEncode+=strTemp;
	}
	
	if (ck.Crack(strEncode.GetString()))
	{
		m_nGestureLength = ck.GetGestureLength();
		memcpy(m_aGesture, ck.GetGesture(), m_nGestureLength*sizeof(&m_aGesture));
		MessageBox(_T("搞定收工！"));
		OnPaint();
		return;
	}
	else
	{
		MessageBox(_T("错误编号 0x002"));
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
		cdc.TextOut(aPoint[m_aGesture[0]].x, aPoint[m_aGesture[0]].y, _T("起点"));
		cdc.SelectObject(tPen);
		for (int i=0;i<m_nGestureLength;i++)
			cdc.LineTo(aPoint[m_aGesture[i]]);
	}
}


void CGestureCrackerDlg::OnBnClickedBtnReDraw()
{
	OnPaint();
}
