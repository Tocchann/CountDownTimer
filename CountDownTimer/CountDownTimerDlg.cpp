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
}

BEGIN_MESSAGE_MAP(CCountDownTimerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON1, &CCountDownTimerDlg::OnBnClickedButton1 )
	ON_REGISTERED_MESSAGE( CCountDownWnd::s_closeCountdownWindow, &CCountDownTimerDlg::OnCloseCountdownWindow )
	ON_BN_CLICKED( IDC_BTN_RESET, &CCountDownTimerDlg::OnBnClickedBtnReset )
END_MESSAGE_MAP()


// CCountDownTimerDlg ���b�Z�[�W �n���h��

BOOL CCountDownTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ZeroMemory( &m_lf, sizeof( m_lf ) );

	m_spin1.SetRange32( 1, 120 );	//	�Œ�2���Ԃ܂�
	m_spin1.SetPos32( 50 );			//	�f�t�H���g��50��
	m_spin2.SetRange32( 0, 59 );
	m_spin2.SetPos32( 0 );			//	�X�^�[�g�܂ł̗]�����Ԃ�0�b	�Z�b�V�����Ȃ̂ŃX�^�[�g���Ԃ̓s�b�^������ł�����Ȃ��B

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
		CCountDownWnd::StartCountDown( start1, start2, &m_lf );
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
	if( m_start1 != 0 || m_start2 != 0 )
	{
		m_spin1.SetPos32( m_start1 );
		m_spin2.SetPos32( m_start2 );
	}
}
