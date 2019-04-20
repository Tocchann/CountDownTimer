// CountDownTimerDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CountDownTimer.h"
#include "CountDownTimerDlg.h"
#include "CountDownWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCountDownTimerDlg ダイアログ




CCountDownTimerDlg::CCountDownTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCountDownTimerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCountDownTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_SPIN1, m_spin1 );
	DDX_Control( pDX, IDC_SPIN2, m_spin2 );
}

BEGIN_MESSAGE_MAP(CCountDownTimerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON1, &CCountDownTimerDlg::OnBnClickedButton1 )
	ON_REGISTERED_MESSAGE( CCountDownWnd::s_closeCountdownWindow, &CCountDownTimerDlg::OnCloseCountdownWindow )
END_MESSAGE_MAP()


// CCountDownTimerDlg メッセージ ハンドラ

BOOL CCountDownTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ZeroMemory( &m_lf, sizeof( m_lf ) );

	m_spin1.SetRange32( 1, 120 );	//	最長2時間まで
	m_spin1.SetPos32( 50 );			//	デフォルトは50分
	m_spin2.SetRange32( 0, 59 );
	m_spin2.SetPos32( 0 );			//	スタートまでの余白時間は0秒	セッションなのでスタート時間はピッタリからでも困らない。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	auto hFont = GetStockObject( DEFAULT_GUI_FONT );
	GetObject( hFont, sizeof( m_lf ), &m_lf );
	SetFontText( m_lf );

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CCountDownTimerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CCountDownTimerDlg::OnOK()
{
	if( UpdateData() ){
		CCountDownWnd::StartCountDown( m_spin1.GetPos32(), m_spin2.GetPos32(), &m_lf );
	}
}
void CCountDownTimerDlg::OnBnClickedButton1()
{
	//	フォントの選択
	LOGFONT* p = &m_lf;
	if( m_lf.lfFaceName[0] == _T( '\0' ) )
	{
		p = nullptr;
	}
	CFontDialog	dlg( p, CF_SCREENFONTS|CF_NOVERTFONTS, nullptr, this );
	if( dlg.DoModal() ==IDOK )
	{
		dlg.GetCurrentFont( &m_lf );
		SetFontText( m_lf );
	}
}
void CCountDownTimerDlg::SetFontText( const LOGFONT& lf )
{
	CString msg;
	LPCTSTR unit = (lf.lfHeight < 0) ? _T( "pt" ) : _T( "dot" ) ;
	msg.Format( _T( "%s(%d%s)" ), lf.lfFaceName, abs( lf.lfHeight ), unit );
	SetDlgItemText( IDC_STC_FONT, msg );
}
LRESULT CCountDownTimerDlg::OnCloseCountdownWindow( WPARAM wParam, LPARAM lParam )
{
	if( wParam > 60 )
	{
		m_spin1.SetPos32( static_cast<int>(wParam/60) );
		m_spin2.SetPos32( static_cast<int>(wParam%60) );
	}
	return 0;
}
