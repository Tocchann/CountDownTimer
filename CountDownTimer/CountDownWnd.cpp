// CowntDownWnd.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CountDownTimer.h"
#include "CountDownWnd.h"

// CCountDownWnd

IMPLEMENT_DYNAMIC( CCountDownWnd, CWnd )
UINT CCountDownWnd::s_closeCountdownWindow = ::RegisterWindowMessage( _T( "CCountDownWnd::ClosedWindow" ) );
BEGIN_MESSAGE_MAP( CCountDownWnd, CWnd )
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_GETFONT()
END_MESSAGE_MAP()

void CCountDownWnd::StartCountDown( _In_ UINT nTimeoutTime, _In_ UINT nPrefixTime, _In_ LOGFONT* plf )
{
	if( plf != nullptr )
	{
		if( plf->lfFaceName[0] == _T( '\0' ) )
		{
			plf = nullptr;
		}
	}
	CString	strCaption;
	strCaption.Format( _T( "�c��...%d��" ), nTimeoutTime );

	CCountDownWnd*	pWnd = new CCountDownWnd();
	pWnd->CalcTickCount( nTimeoutTime, nPrefixTime );
	if( pWnd->CreateEx( WS_EX_TOPMOST, nullptr, strCaption, WS_POPUP, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, plf ) ){
		theApp.m_pMainWnd->ShowWindow( SW_HIDE );
		pWnd->ShowWindow( SW_SHOWNOACTIVATE );
		pWnd->UpdateWindow();
	}
	else{
		delete pWnd;
		AfxMessageBox( _T("�ȁA�Ȃ�Ƃ������Ƃ��I\n\n�E�B���h�E�����܂���B") );
	}
}

CCountDownWnd::CCountDownWnd()
:	m_timerID( 0 )
{
	m_brDefBack.CreateSolidBrush( RGB(255,0,0) );
	m_brTimeoutBack.CreateSolidBrush( RGB(128,0,128) );
}

CCountDownWnd::~CCountDownWnd()
{
}

BOOL CCountDownWnd::PreCreateWindow( CREATESTRUCT& cs )
{
	//	�T�C�Y�ύX�ōĕ`��E�J�[�\���͎�A�w�i�͂Ƃ肠�����f�t�H���g�o�b�N�J���[
	if( cs.lpszClass == NULL )
	{
		cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW, LoadCursor( NULL, IDC_HAND ), m_brDefBack, NULL );
	}
	//	���[�N�G���A�̍ŏ㕔�ɒ���t����
	CRect rc;
	SystemParametersInfo( SPI_GETWORKAREA, sizeof( RECT ), &rc, 0 );
	cs.x = rc.left;
	cs.y = rc.top;
	cs.cx = rc.Width();
	cs.cy = rc.Height()/100;	//	�ŏ�
	//	�t�H���g�T�C�Y���m��
	if( m_fntTimer.m_hObject == nullptr && cs.lpCreateParams != nullptr )
	{
		auto plp = static_cast<LOGFONT*>(cs.lpCreateParams);
		if( m_fntTimer.CreateFontIndirect( plp ) )
		{
			LOGFONT lfCurr;
			m_fntTimer.GetLogFont( &lfCurr );
			//	�t�H���g�̂ق����傫���ꍇ�͏��������Ă��܂�
			if( lfCurr.lfHeight >= cs.cy )
			{
				lfCurr.lfHeight = cs.cy-1;
				m_fntTimer.DeleteObject();
				if( m_fntTimer.CreateFontIndirect( &lfCurr ) )
				{
					plp->lfHeight = lfCurr.lfHeight;
				}
			}
		}
	}
	auto pFont = &m_fntTimer;
	//	�^�C�}�[���Ԃ��\���ł���傫��������΂���
	{
		CClientDC	dc( AfxGetMainWnd() );
		auto pOld = dc.SelectObject( pFont );
		CSize	sizeText = dc.GetTextExtent( _T( "-0123456789:" ) );	//	���p����e�L�X�g��S�����ׂĂ���
		cs.cy = sizeText.cy;	//	�������ɐݒ肵�Ă����ł��傤
		dc.SelectObject( pOld );
	}
	return CWnd::PreCreateWindow( cs );
}

// CCountDownWnd ���b�Z�[�W �n���h��
int CCountDownWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;
	m_startTime = 0;
	//	�^�C�}�[���Z�b�g�ł��Ȃ��ꍇ�͏I���
	m_timerID = SetTimer( AFX_IDM_WINDOW_FIRST, 200, nullptr );	//	�J�E���g�_�E����0.2�b
	if( m_timerID == 0 )
	{
		AfxMessageBox( _T( "���[��...orz\n\n�^�C�}�[�������܂���B" ) );
		return -1;
	}
	return 0;
}
void CCountDownWnd::OnTimer(UINT_PTR nIDEvent)
{
	if( m_timerID == nIDEvent )
	{
		CRect	rc;
		GetWindowRect( &rc );
		if( m_startTime == 0 )
		{
			m_sizeWnd = rc.Size();
			m_startTime = GetTickCount64();
			m_courseTime = 0;
		}
		auto resultTime = static_cast<UINT>( GetTickCount64()-m_startTime );
		bool countDown = m_courseTime != resultTime/1000 ;
		m_courseTime = resultTime/1000;	//	�~���b�P�ʂȂ̂ŕb�ɒ���
		UINT	restTime;
		int cxTime = m_totalTime*1000;
		if( m_courseTime < m_totalTime )
		{
			restTime = m_totalTime-m_courseTime;
			cxTime -= resultTime;
		}
		else
		{
			restTime = m_courseTime-m_totalTime;
			cxTime = resultTime-cxTime;
		}
		//	�J�E���g�_�E������ꍇ(�e�L�X�g���ς��)
		if( countDown )
		{
			//	�o�ߎ��ԓ�
			CString	strCaption;
			if( m_courseTime < m_totalTime )
			{
				if( restTime > 60+30 )
				{
					strCaption.Format( _T( "�c��:%d��%d�b" ), restTime/60, restTime%60 );
				}
				else
				{
					strCaption.Format( _T( "�c��:%d�b" ), restTime );
				}
			}
			//	���Ԓ��߁I
			else
			{
				//	�\�莞�Ԃ̓�{�܂Ői�񂾂狭���I���I
				if( restTime > m_totalTime )
				{
					KillTimer( m_timerID );
					m_timerID = 0;
					PostMessage( WM_CLOSE );
					return;
				}
				if( restTime >= 60 )
				{
					strCaption.Format( _T( "%d �� %d �b���Ԓ���" ), restTime/60, restTime%60 );
				}
				else
				{
					strCaption.Format( _T( "%d �b���Ԓ���" ), restTime );
				}
			}
			SetWindowText( strCaption );
		}
		//	�E�B���h�E�̓C���W�P�[�^�̖������ʂ����̂ŁA���̂܂܃Q�[�W��L�яk�݂�����
		UINT	width = MulDiv( m_sizeWnd.cx, cxTime, m_totalTime*1000 );	//	�E�B���h�E�T�C�Y�̍��݂͂��������ׂ�������
		TRACE( _T( "%d:%4d-%02d:%02d\n" ), m_sizeWnd.cx, width, restTime/60, restTime%60 );
		if( rc.Width() != width )
		{
			SetWindowPos( NULL, 0, 0, width, m_sizeWnd.cy, SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED );
		}
		else if( countDown )
		{
			Invalidate();
		}
	}
	else
	{
		CWnd::OnTimer( nIDEvent );
	}
}

void CCountDownWnd::PostNcDestroy()
{
	CWnd::PostNcDestroy();
	delete this;	//	�E�B���h�E���j�����ꂽ�玩�����폜
	theApp.m_pMainWnd->ShowWindow( SW_SHOW );
}
HFONT CCountDownWnd::OnGetFont()
{
	HFONT hFont = static_cast<HFONT>(m_fntTimer.m_hObject);
	if( hFont == nullptr )
	{
		hFont = static_cast<HFONT>(GetStockObject( DEFAULT_GUI_FONT ));
	}
	return hFont;
}
void CCountDownWnd::OnPaint()
{
	CPaintDC dc( this );
	auto pFont = GetFont();
	auto pOld = (pFont != nullptr)? dc.SelectObject( pFont ) : nullptr ;
	CRect	rc;
	GetClientRect( rc );
	dc.SetTextColor( RGB( 255, 255, 255 ) );
	dc.SetBkMode( TRANSPARENT );
	CString	strText;
	UINT	restTime = (m_courseTime < m_totalTime) ? m_totalTime-m_courseTime : m_courseTime-m_totalTime ;
	if( restTime >= 60 )
	{
		strText.Format( _T( "%d:%02d" ), restTime/60, restTime%60 );
	}
	else
	{
		strText.Format( _T( "%d" ), restTime%60 );
	}
	dc.TextOut( 0, 0, strText, strText.GetLength() );
	if( pOld != nullptr )
	{
		dc.SelectObject( pOld );
	}
}

BOOL CCountDownWnd::OnEraseBkgnd(CDC* pDC)
{
	if( m_totalTime <= m_courseTime )
	{
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
	if( GetCapture() == nullptr )
	{
		SetCapture();
	}
}
void CCountDownWnd::OnRButtonUp( UINT nFlags, CPoint point )
{
	if( GetCapture() == this )
	{
		ReleaseCapture();
		CRect	rc;
		GetWindowRect( &rc );
		if( rc.PtInRect( point ) )
		{
			if( m_courseTime < m_totalTime )
			{
				AfxGetMainWnd()->PostMessage( s_closeCountdownWindow, m_totalTime-m_courseTime );
			}
			PostMessage( WM_CLOSE );
		}
	}
	else
	{
		CWnd::OnRButtonUp( nFlags, point );
	}
}
void CCountDownWnd::OnDestroy()
{
	if( m_timerID != 0 ){
		KillTimer( m_timerID );
		m_timerID = 0;
	}
	CWnd::OnDestroy();
}
