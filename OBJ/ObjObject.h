﻿#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>         
#include <string>

#include "ObjModel.h"
#include "Camera.h"

class BaseCollider;

// 3Dオブジェクト
class ObjObject
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // サブクラス	

	// パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMFLOAT4 color;		//色
		XMMATRIX mat;	// ３Ｄ変換行列
	};

private: // 定数


public: // 静的メンバ関数
	// 静的初期化
	static void StaticInitialize( ID3D12Device *device, Camera *camera = nullptr);

	// グラフィックパイプラインの生成
	static void CreateGraphicsPipeline();
	
	// カメラのセット
	static void SetCamera( Camera *camera )
	{
		ObjObject::camera = camera;
	}
	
	// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	
	// 描画後処理
	static void PostDraw();

	// 3Dオブジェクト生成
	static ObjObject* Create( ObjModel *model = nullptr );


private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// テクスチャあり用パイプライン
	static PipelineSet pipelineSet;
public:
	// カメラ
	static Camera *camera;
	
public: // メンバ関数
	// コンストラクタ
	ObjObject() = default;

	// デストラクタ
	virtual ~ObjObject();

	// 初期化
	virtual bool Initialize();

	// 毎フレーム処理
	virtual void Update();

	// 描画
	virtual void Draw();

	// ワールド行列の取得
	const XMMATRIX& GetMatWorld() { return matWorld; }

	// 座標の取得
	const XMFLOAT3& GetPosition() { return position; }

	// 座標の取得
	const XMFLOAT3& GetRotation() { return rotation; }

	//スケールの取得
	const XMFLOAT3& GetScale() { return scale; }

	// 色の取得
	const XMFLOAT4& GetColor() { return color; }

	// ワールド座標の習得
	const XMFLOAT3 GetWorldPosition()
	{
		XMVECTOR positionV;
		XMFLOAT3 worldPosition;
		positionV = DirectX::XMLoadFloat3(&position);
		positionV.m128_f32[3] = 1.0f;
		positionV = DirectX::XMVector3Transform(positionV, parent->matWorld);
		DirectX::XMStoreFloat3(&worldPosition, positionV);
		return worldPosition;
	}

	// 座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	// 角度の設定
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	// スケールの設定
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	// 色の設定
	void SetColor(const XMFLOAT4& color) { this->color = color; }

	// モデルのセット
 	void SetModel(ObjModel* model) { this->model = model; }

	void SetParent(ObjObject* parent) { this->parent = parent; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

protected: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	ObjObject* parent = nullptr;
	// 
	XMMATRIX updateMatWorld;
	// モデル
	ObjModel* model = nullptr;

	// ビルボード
	bool isBillboard = false;

	// クラス名
	const char* name = nullptr;
	// コライダー
	BaseCollider* collider = nullptr;
};

