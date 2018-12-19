// CountDownTimer.cpp : アプリケーションのクラス動作を定義します。
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


// CCountDownTimerApp コンストラクション

CCountDownTimerApp::CCountDownTimerApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CCountDownTimerApp オブジェクトです。

CCountDownTimerApp theApp;


// CCountDownTimerApp 初期化

BOOL CCountDownTimerApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	//	プライベートフォントをリストアップする
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
	*ATLPath::FindFileName( modulePath ) = _T( '\0' );	//	ファイル名で切ってフォルダにする
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
				AddFontResourceEx( filePath, FR_PRIVATE, nullptr );	//	エラーは無視
			}
		} while( FindNextFile( findFile, &findData ) );
		FindClose( findFile );
	}
}
