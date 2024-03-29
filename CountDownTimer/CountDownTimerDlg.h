// CountDownTimerDlg.h : ヘッダー ファイル
//

#pragma once


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
	HICON	m_hIcon;
	LOGFONT	m_lf;
	CFont	m_fontTimer;
	int		m_start1;
	int		m_start2;
	bool	m_blockReset;

	std::list<MONITORINFOEX> m_monitorInfos;

	// 生成された、メッセージ割り当て関数
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
	afx_msg void OnBnClickedBtnReset();
	CComboBox m_combMonitor;
	afx_msg void OnEnUpdateEdit();
};
