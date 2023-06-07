﻿#pragma once
#include <Windows.h>

#define WINDOW_H_
#define WINDOW_CLASS_NAME "Gear Break" // ウィンドウクラス名

// ウィンドウズアプリケーション
class WinApp
{
public: // 静的メンバ変数
	// ウィンドウサイズ
	static const int window_width = 1280;	// 横幅
	static const int window_height = 720;	// 縦幅
	static const wchar_t windowClassName[];

public: // 静的メンバ関数
	// ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数
	// ゲームウィンドウの作成
	void CreateGameWindow();

	// ゲームウィンドウの破棄
	void TerminateGameWindow();

	// メッセージの処理
	bool ProcessMessage();

	// ウィンドウハンドルの取得
	HWND GetHwnd() { return hwnd; }

	HINSTANCE GetInstance() { return wndClass.hInstance; }

	static WinApp* GetInst();

private: // メンバ変数
	// Window関連
	HWND hwnd = nullptr;	// ウィンドウハンドル
	WNDCLASSEX wndClass{};			// ウィンドウクラス
};

