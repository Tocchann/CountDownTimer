// CountDownTimer.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "CountDownTimer.h"
#include "CountDownTimerDlg.h"

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

	CCountDownTimerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
