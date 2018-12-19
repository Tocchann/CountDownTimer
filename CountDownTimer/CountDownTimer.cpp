// CountDownTimer.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "CountDownTimer.h"
#include "CountDownTimerDlg.h"
#include <atlpath.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCountDownTimerApp

BEGIN_MESSAGE_MAP(CCountDownTimerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCountDownTimerApp �R���X�g���N�V����

CCountDownTimerApp::CCountDownTimerApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� CCountDownTimerApp �I�u�W�F�N�g�ł��B

CCountDownTimerApp theApp;


// CCountDownTimerApp ������

BOOL CCountDownTimerApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	//	�v���C�x�[�g�t�H���g�����X�g�A�b�v����
	AddPrivateFonts();

	CCountDownTimerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
void CCountDownTimerApp::AddPrivateFonts()
{
	TCHAR	modulePath[MAX_PATH];
	GetModuleFileName( nullptr, modulePath, MAX_PATH );
	*ATLPath::FindFileName( modulePath ) = _T( '\0' );	//	�t�@�C�����Ő؂��ăt�H���_�ɂ���
	CPath	pathFonts;
	pathFonts.Combine( modulePath, _T( "Fonts" ) );
	WIN32_FIND_DATA	findData;
	auto findFile = ::FindFirstFile( pathFonts.m_strPath + _T( "\\*.*" ), &findData );
	if( findFile != INVALID_HANDLE_VALUE )
	{
		do
		{
			if( (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
			{
				CPath	filePath;
				filePath.Combine( pathFonts, findData.cFileName );
				AddFontResourceEx( filePath, FR_PRIVATE, nullptr );	//	�G���[�͖���
			}
		} while( FindNextFile( findFile, &findData ) );
		FindClose( findFile );
	}
}
