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
	, m_lf{ 0 }
	, m_start1(0)
	, m_start2(0)
	, m_blockReset( false )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCountDownTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_SPIN1, m_spin1 );
	DDX_Control( pDX, IDC_SPIN2, m_spin2 );
	DDX_Control( pDX, IDC_COMBO_MONITOR, m_combMonitor );
}

BEGIN_MESSAGE_MAP(CCountDownTimerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON1, &CCountDownTimerDlg::OnBnClickedButton1 )
	ON_REGISTERED_MESSAGE( CCountDownWnd::s_closeCountdownWindow, &CCountDownTimerDlg::OnCloseCountdownWindow )
	ON_BN_CLICKED( IDC_BTN_RESET, &CCountDownTimerDlg::OnBnClickedBtnReset )
	ON_EN_UPDATE( IDC_EDIT1, &CCountDownTimerDlg::OnEnUpdateEdit )
	ON_EN_UPDATE( IDC_EDIT2, &CCountDownTimerDlg::OnEnUpdateEdit )
END_MESSAGE_MAP()


// CCountDownTimerDlg メッセージ ハンドラ

BOOL CCountDownTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		if( strAboutMenu.LoadString( IDS_ABOUTBOX ) && !strAboutMenu.IsEmpty() )
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	//	時間のエディットの初期値設定
	m_blockReset = true;
	m_spin1.SetRange32( 1, 120 );	//	最長2時間まで
	m_spin1.SetPos32( 50 );			//	デフォルトは50分
	m_spin2.SetRange32( 0, 59 );
	m_spin2.SetPos32( 0 );			//	スタートまでの余白時間は0秒	セッションなのでスタート時間はピッタリからでも困らない。
	m_blockReset = false;

	//	デフォルトは、キャプションフォントに合わせる
	NONCLIENTMETRICS metric ={ 0 };
	metric.cbSize = sizeof( metric );
	if( SystemParametersInfo( SPI_GETNONCLIENTMETRICS, metric.cbSize, &metric, 0 ) )
	{
		m_lf = metric.lfCaptionFont;
	}
	else
	{
		auto hFont = GetStockObject( DEFAULT_GUI_FONT );
		GetObject( hFont, sizeof( m_lf ), &m_lf );
	}
	//CClientDC dc( this );
	//auto dpi = dc.GetDeviceCaps( LOGPIXELSY );
	//m_lf.lfHeight = MulDiv( m_lf.lfHeight, dpi, 96 );	//	デフォルトDPIの値のはずなので換算しなおす
	SetFontText( m_lf );

	//	モニターをリストアップして、コンボに設定(ラムダはなにもキャプチャしなければ関数コールに置き換えてくれる)
	EnumDisplayMonitors( nullptr, nullptr, []( HMONITOR hMon, HDC, LPRECT, LPARAM lParam )
	{
		std::list<MONITORINFOEX>& monitorInfos = *reinterpret_cast<std::list<MONITORINFOEX>*>(lParam);
		MONITORINFOEX info={};
		info.cbSize = sizeof( info );
		if( GetMonitorInfo( hMon, &info ) )
		{
			monitorInfos.push_back( info );
		}
		return TRUE;
	}, reinterpret_cast<LPARAM>(&m_monitorInfos) );
	
	int curSel = -1;
	for( const auto& info : m_monitorInfos )
	{
		int index = m_combMonitor.AddString( info.szDevice );
		if( info.dwFlags == MONITORINFOF_PRIMARY )
		{
			curSel = index;
		}
	}
	m_combMonitor.SetCurSel( curSel );
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
#include <ShellScalingApi.h>
#pragma comment(lib,"Shcore.lib")

void CCountDownTimerDlg::OnOK()
{
	if( UpdateData() ){
		auto start1 = m_spin1.GetPos32();
		auto start2 = m_spin2.GetPos32();
		if( m_start1 == 0 && m_start2 == 0 )
		{
			m_start1 = start1;
			m_start2 = start2;
		}
		int curSel = m_combMonitor.GetCurSel();
		CString selDeviceName;
		m_combMonitor.GetLBText( curSel, selDeviceName );
		CRect rcWindow;
		for( const auto& info : m_monitorInfos )
		{
			if( selDeviceName == info.szDevice )
			{
				rcWindow = info.rcWork;
				break;
			}
		}
		auto hMon = MonitorFromRect( &rcWindow, MONITOR_DEFAULTTONEAREST );
		UINT xDPI, yDPI;
		GetDpiForMonitor( hMon, MDT_DEFAULT, &xDPI, &yDPI );

		//	ダイアログがある側のモニターサイズで算出されているはずなので、計算しなおしてやる必要がある
		CClientDC dc( this );
		LOGFONT lf = m_lf;
		lf.lfHeight = MulDiv( lf.lfHeight, yDPI, dc.GetDeviceCaps( LOGPIXELSY ) );
		CCountDownWnd::StartCountDown( start1, start2, &lf, rcWindow );
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
	CClientDC dc( this );
	int dpi = dc.GetDeviceCaps( LOGPIXELSY );
	int height = abs( MulDiv( lf.lfHeight, 72, dpi ) );
	msg.Format( _T( "%s(%dpt)" ), lf.lfFaceName, height );
	SetDlgItemText( IDC_STC_FONT, msg );
}
LRESULT CCountDownTimerDlg::OnCloseCountdownWindow( WPARAM wParam, LPARAM lParam )
{
	if( wParam > 60 )
	{
		m_blockReset = true;
		m_spin1.SetPos32( static_cast<int>(wParam/60) );
		m_spin2.SetPos32( static_cast<int>(wParam%60) );
		m_blockReset = false;
	}
	return 0;
}
void CCountDownTimerDlg::OnBnClickedBtnReset()
{
	m_blockReset = true;
	//	前回タイマースタートの時間があった場合はそれに戻す
	if( m_start1 != 0 || m_start2 != 0 )
	{
		m_spin1.SetPos32( m_start1 );
		m_spin2.SetPos32( m_start2 );
		//	一度リセットしたら、覚えた値は忘れる
		m_start1 = m_start2 = 0;
	}
	//	設定がない場合は初期値を再設定
	else
	{
		m_spin1.SetPos32( 50 );
		m_spin2.SetPos32( 0 );
	}
	m_blockReset = false;
}


void CCountDownTimerDlg::OnEnUpdateEdit()
{
	if( !m_blockReset )
	{
		m_start1 = m_start2 = 0;
	}
}
