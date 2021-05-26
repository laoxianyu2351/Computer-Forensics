
// GestureCrackerDlg.h : 头文件
//

#pragma once


// CGestureCrackerDlg 对话框
class CGestureCrackerDlg : public CDialogEx
{
// 构造
public:
	CGestureCrackerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GESTURECRACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnCrack();
	afx_msg void OnBnClickedBtnReDraw();
private:
	// gesture.key文件路径
	CString m_strFileName;
	int m_aGesture[9];
	int m_nGestureLength;

private:
	void DrawGesture(void);
};
