// CountDownTimer.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CCountDownTimerApp:
// このクラスの実装については、CountDownTimer.cpp を参照してください。
//

class CCountDownTimerApp : public CWinApp
{
public:
	CCountDownTimerApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CCountDownTimerApp theApp;