﻿#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

// 入力
class Input
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 静的メンバ関数
	static Input* GetInstance();

public: // メンバ関数
	// 初期化
	bool Initialize(HINSTANCE hInstance, HWND hwnd);

	// 毎フレーム処理
	void Update();

	// キーの押下をチェック
	bool PushKey(BYTE keyNumber);

	// キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);

private: // メンバ変数
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

protected:
	Input() = default;

	~Input() = default;

public:
	Input(Input & other) = delete;

	void operator=(const Input&) = delete;
};

