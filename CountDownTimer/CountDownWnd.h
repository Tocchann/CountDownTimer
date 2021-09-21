#pragma once


// CCountDownWnd

class CCountDownWnd : public CWnd
{
	DECLARE_DYNAMIC(CCountDownWnd)

public:
	static UINT s_closeCountdownWindow;
static	void StartCountDown( _In_ UINT nTimeoutTime, _In_ UINT nPrefixTime, _In_ LOGFONT* plf, const CRect& rcWindow );	//	�G���[�����͗�O��

private:
	CCountDownWnd();
	virtual ~CCountDownWnd();

	void	CalcTickCount( _In_ UINT nTimeoutTime, _In_ UINT nPrefixTime )
	{
		m_totalTime = nTimeoutTime*60 + nPrefixTime;	//	�b�ŃZ�b�g�����
		m_courseTime = 0;
	}

	CRect		m_rcWindow;		//	�ΏۃE�B���h�E�̓��[�U�[�w��
	UINT_PTR	m_timerID;
	UINT		m_totalTime;	//	��������(�b)
	UINT		m_courseTime;	//	�o�ߎ���(�b)
	ULONGLONG	m_startTime;	//	�J�E���g�_�E���J�n����
	CSize		m_sizeWnd;		//	�E�B���h�E�T�C�Y

	CBrush		m_brDefBack;
	CBrush		m_brTimeoutBack;
	CFont		m_fntTimer;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnPaint();
	afx_msg HFONT OnGetFont();
};


