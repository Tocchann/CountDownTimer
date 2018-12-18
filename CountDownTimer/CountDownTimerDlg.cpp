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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCountDownTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_SPIN2, m_spin2 );
	DDX_Control( pDX, IDC_DATETIMEPICKER1, m_timeoutTime );
}

BEGIN_MESSAGE_MAP(CCountDownTimerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CCountDownTimerDlg ���b�Z�[�W �n���h��

BOOL CCountDownTimerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CTime nowTime = CTime::GetCurrentTime();
	CTime timerTime( nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay(), 0, 50, 0 );
	m_timeoutTime.SetTime( &timerTime );
	m_spin2.SetRange( 0, 100 );
	m_spin2.SetPos( 0 );	//	1024�ȃX�N���[������1%�ŁA10�s�N�Z�����x

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

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
		CTime time;
		m_timeoutTime.GetTime( time );
		UINT timeoutTime = time.GetHour() * 60 * 60 * 1000;
		timeoutTime += time.GetMinute() * 60 * 1000;
		timeoutTime += time.GetSecond() * 1000;
		
		CCountDownWnd::StartCountDown( timeoutTime, m_spin2.GetPos() );
	}
}
