// CountDownTimerDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CountDownTimer.h"
#include "CountDownTimerDlg.h"
#include "CountDownWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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


// CCountDownTimerDlg �_�C�A���O




CCountDownTimerDlg::CCountDownTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCountDownTimerDlg::IDD, pParent)
	, m_lf{ 0 }
	, m_start1(0)
	, m_start2(0)
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


// CCountDownTimerDlg ���b�Z�[�W �n���h��

BOOL CCountDownTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	//	���Ԃ̃G�f�B�b�g�̏����l�ݒ�
	m_spin1.SetRange32( 1, 120 );	//	�Œ�2���Ԃ܂�
	m_spin1.SetPos32( 50 );			//	�f�t�H���g��50��
	m_spin2.SetRange32( 0, 59 );
	m_spin2.SetPos32( 0 );			//	�X�^�[�g�܂ł̗]�����Ԃ�0�b	�Z�b�V�����Ȃ̂ŃX�^�[�g���Ԃ̓s�b�^������ł�����Ȃ��B

	//	�f�t�H���g�́A�L���v�V�����t�H���g�ɍ��킹��
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
	//m_lf.lfHeight = MulDiv( m_lf.lfHeight, dpi, 96 );	//	�f�t�H���gDPI�̒l�̂͂��Ȃ̂Ŋ��Z���Ȃ���
	SetFontText( m_lf );

	//	���j�^�[�����X�g�A�b�v���āA�R���{�ɐݒ�(�����_�͂Ȃɂ��L���v�`�����Ȃ���Ί֐��R�[���ɒu�������Ă����)
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
	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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

		//	�_�C�A���O�����鑤�̃��j�^�[�T�C�Y�ŎZ�o����Ă���͂��Ȃ̂ŁA�v�Z���Ȃ����Ă��K�v������
		CClientDC dc( this );
		LOGFONT lf = m_lf;
		lf.lfHeight = MulDiv( lf.lfHeight, yDPI, dc.GetDeviceCaps( LOGPIXELSY ) );
		CCountDownWnd::StartCountDown( start1, start2, &lf, rcWindow );
	}
}
void CCountDownTimerDlg::OnBnClickedButton1()
{
	//	�t�H���g�̑I��
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
		m_spin1.SetPos32( static_cast<int>(wParam/60) );
		m_spin2.SetPos32( static_cast<int>(wParam%60) );
	}
	return 0;
}
void CCountDownTimerDlg::OnBnClickedBtnReset()
{
	//	�O��^�C�}�[�X�^�[�g�̎��Ԃ��������ꍇ�͂���ɖ߂�
	if( m_start1 != 0 || m_start2 != 0 )
	{
		m_spin1.SetPos32( m_start1 );
		m_spin2.SetPos32( m_start2 );
		//	��x���Z�b�g������A�o�����l�͖Y���
		m_start1 = m_start2 = 0;
	}
	//	�ݒ肪�Ȃ��ꍇ�͏����l���Đݒ�
	else
	{
		m_spin1.SetPos32( 50 );
		m_spin2.SetPos32( 0 );
	}
}


void CCountDownTimerDlg::OnEnUpdateEdit()
{
	m_start1 = m_start2 = 0;
}
