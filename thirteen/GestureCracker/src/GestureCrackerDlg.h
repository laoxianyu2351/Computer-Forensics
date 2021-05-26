
// GestureCrackerDlg.h : ͷ�ļ�
//

#pragma once


// CGestureCrackerDlg �Ի���
class CGestureCrackerDlg : public CDialogEx
{
// ����
public:
	CGestureCrackerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GESTURECRACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnCrack();
	afx_msg void OnBnClickedBtnReDraw();
private:
	// gesture.key�ļ�·��
	CString m_strFileName;
	int m_aGesture[9];
	int m_nGestureLength;

private:
	void DrawGesture(void);
};
