// CountDownTimerDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxcmn.h"


// CCountDownTimerDlg �_�C�A���O
class CCountDownTimerDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CCountDownTimerDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^

// �_�C�A���O �f�[�^
	enum { IDD = IDD_COUNTDOWNTIMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON	m_hIcon;
	LOGFONT	m_lf;
	CFont	m_fontTimer;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	void SetFontText( const LOGFONT& lf );
public:
	CSpinButtonCtrl m_spin1;
	CSpinButtonCtrl m_spin2;
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnCloseCountdownWindow( WPARAM, LPARAM );
};
