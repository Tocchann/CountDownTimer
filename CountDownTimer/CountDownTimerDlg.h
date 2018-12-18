// CountDownTimerDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"


// CCountDownTimerDlg ダイアログ
class CCountDownTimerDlg : public CDialog
{
// コンストラクション
public:
	CCountDownTimerDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_COUNTDOWNTIMER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CSpinButtonCtrl m_spin2;
	CDateTimeCtrl m_timeoutTime;
};
