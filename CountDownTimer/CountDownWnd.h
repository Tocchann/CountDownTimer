#pragma once


// CCountDownWnd

class CCountDownWnd : public CWnd
{
	DECLARE_DYNAMIC(CCountDownWnd)

public:
static	void StartCountDown( UINT nTimeoutTime, int size );	//	ÉGÉâÅ[î≠ê∂ÇÕó·äOÇó

private:
	CCountDownWnd();
	virtual ~CCountDownWnd();

	UINT_PTR	m_timerID;
	UINT	m_resizeStep;
	bool	m_bOver;

	CBrush	m_brDefBack;
	CBrush	m_brTimeoutBack;
	CToolTipCtrl	m_toolTip;
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
};


