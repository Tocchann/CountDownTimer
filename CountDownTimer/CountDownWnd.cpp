// CowntDownWnd.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CountDownTimer.h"
#include "CountDownWnd.h"

// CCountDownWnd

IMPLEMENT_DYNAMIC(CCountDownWnd, CWnd)

BEGIN_MESSAGE_MAP( CCountDownWnd, CWnd )
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CCountDownWnd::StartCountDown( UINT nTimeoutTime, int size )
{
	UINT	cx = GetSystemMetrics( SM_CXSCREEN );
	UINT	cy = GetSystemMetrics( SM_CYSCREEN );
	UINT	cxWnd = (size == 0) ? 4 : MulDiv( cx, size, 100 );

	CCountDownWnd*	pWnd = new CCountDownWnd();
	if( pWnd->CreateEx( WS_EX_TOPMOST, NULL, NULL, WS_POPUP, 0, 0, cxWnd, cy, NULL, NULL ) ){
		UINT	tickCount;
		//	�s�N�Z���ϓ���1�b�ȏ�J���ꍇ
		if( nTimeoutTime > cy*1000 ){
			pWnd->m_resizeStep = 1;
			tickCount = nTimeoutTime/cy;	//	1�s�N�Z�������邽�߂ɕK�v�ȃ^�C�}�[����
		}
		else{
			pWnd->m_resizeStep = cy/(nTimeoutTime/1000);
			tickCount = 1000;
		}
		pWnd->m_timerID = pWnd->SetTimer( cx, tickCount, NULL );
		if( pWnd->m_timerID != 0 ){
			theApp.m_pMainWnd->ShowWindow( SW_HIDE );
			pWnd->ShowWindow( SW_SHOWNOACTIVATE );
			pWnd->UpdateWindow();
		}
		else{
			pWnd->DestroyWindow();
			AfxMessageBox( _T("�^�C�}�[�������܂���B���肦�ˁ[�ł��B�̂Ă��Ⴆ�I����ȃ}�V���͂�����") );
		}
	}
	else{
		delete pWnd;
		AfxMessageBox( _T("�E�B���h�E�����܂���B���肦�ˁ[�ł��B����Ȃւ���}�V��...�͂�") );
	}
}

CCountDownWnd::CCountDownWnd()
:	m_timerID( 0 )
,	m_bOver( false )
{
	m_brDefBack.CreateSolidBrush( RGB(255,0,0) );
	m_brTimeoutBack.CreateSolidBrush( RGB(128,0,128) );
}

CCountDownWnd::~CCountDownWnd()
{
}

BOOL CCountDownWnd::PreCreateWindow( CREATESTRUCT& cs )
{
	if( cs.lpszClass == NULL )
	{
		cs.lpszClass = AfxRegisterWndClass( CS_VREDRAW, NULL, m_brDefBack, NULL );
	}
	return CWnd::PreCreateWindow( cs );
}

// CCountDownWnd ���b�Z�[�W �n���h��
void CCountDownWnd::OnTimer(UINT_PTR nIDEvent)
{
	if( m_timerID == nIDEvent ){
		CRect	rc;
		GetWindowRect( &rc );
		if( rc.Height() < (int)m_resizeStep && !m_bOver ){
			m_bOver = true;
		}
		if( m_bOver ){
			if( rc.top < (int)m_resizeStep ){
				KillTimer( m_timerID );	//	�^�C�}�[�~�߂�I
				m_timerID = 0;
				PostMessage( WM_CLOSE );
			}
			else{
				SetWindowPos( NULL, rc.left, rc.top-m_resizeStep, rc.Width(), rc.Height()+m_resizeStep, SWP_NOZORDER|SWP_FRAMECHANGED );
			}
		}
		else{
			SetWindowPos( NULL, rc.left, rc.top+m_resizeStep, rc.Width(), rc.Height()-m_resizeStep, SWP_NOZORDER|SWP_FRAMECHANGED );
		}
	}
	else{
		CWnd::OnTimer(nIDEvent);
	}
}

void CCountDownWnd::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	delete this;	//	�E�B���h�E���j�����ꂽ�玩�����폜
	theApp.m_pMainWnd->ShowWindow( SW_SHOW );
}

BOOL CCountDownWnd::OnEraseBkgnd(CDC* pDC)
{
	if( m_bOver ){
		CRect	rc;
		pDC->GetClipBox( &rc );
		pDC->FillRect( &rc, &m_brTimeoutBack );
		return TRUE;
	}
	return CWnd::OnEraseBkgnd(pDC);
}

void CCountDownWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown( nFlags, point );
	SetCapture();
	//DestroyWindow();
}
void CCountDownWnd::OnRButtonUp( UINT nFlags, CPoint point )
{
	if( GetCapture() == this )
	{
		ReleaseCapture();
		PostMessage( WM_CLOSE );
	}
	else
	{
		CWnd::OnRButtonUp( nFlags, point );
	}
}
int CCountDownWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	return 0;
}
void CCountDownWnd::OnDestroy()
{
	if( m_timerID != 0 ){
		KillTimer( m_timerID );
		m_timerID = 0;
	}
	CWnd::OnDestroy();
}
