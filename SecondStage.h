#pragma once

#include "BaseScene.h"

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "DebugText.h"
#include "ScoreText.h"
#include "Sound.h"
#include "FbxObject.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "FbxLoader.h"
#include "FbxObject.h"
#include "WinApp.h"

#include "Player.h"
#include "Bullet.h"
#include "Boss.h"
#include "BossTurret.h"
#include "BossCore.h"
#include "BossMainCore.h"
#include "BossParts.h"
#include "BossShield.h"
#include "TargetBullet.h"
#include "ObjectParticle.h"
#include "ReflectBullet.h"

#include "Spline.h"
#include "Lerp.h"
#include "Easing.h"

#include "SceneManager.h"

#include "GageUI.h"
#include "DeltaGageUI.h"
#include "Collision.h"

#include "MeterUI.h"
#include "ProcessUI.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>
#include <list>
#include <array>

class CollisionManager;
class Player;
class Bullet;
class PlayerSpecialBullet;
class Boss;
class BossBullet;
class TargetBullet;
class StageObject;
class BossCore;
class BossMainCore;
class BossParts;
class BossTurret;
class GageUI;
class DeltaGageUI;
class ReflectBullet;
class BossShield;
class MeterUI;
class ProcessUI;

class SecondStage : public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
// �X�v���C�g�̃e�N�X�`���ԍ�
	static enum TextureNumber
	{
		debug_txt,
		game_bg,
		reticle,
		black,
		rule,
		speed,
		meter,
		process,
		breakshield,
		breakmaincore,
		breakcore,

		// �{�X
		game_boss_frame_1,
		game_boss_frame_2,
		game_boss_gauge
	};

	// �v���C���[�̋�
	static enum BulletType
	{
		Normal,
		Charge
	};

public:
	// �R���X�g���N�^
	SecondStage();

	// �f�X�g���N�^
	~SecondStage();

	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;

	//�}�E�X���擾
	void GetMouse();

	//�f�o�b�O�e�L�X�g�p�֐�
	void DrawDebugText();

	// �v���C���[�̒e�𔭎�
	void Shoot();

	void chargeShoot();

	void PlayerMovementBoundaryChecking();

	// �{�X�̒e�𔭎�
	void BossTargetShoot(XMFLOAT3 startPosition, XMFLOAT3 endPosition, float bulletSpeed);

	// �{�X�̒e�𔭎�
	void BossReflectShoot(XMFLOAT3 startPosition, XMFLOAT3 endPosition, float bulletSpeed);

	// �R�A���j�G�t�F�N�g
	void CoreBreakEffect();

	// �R�A�q�b�g�G�t�F�N�g
	void CoreHitEffect();

	// �{�X�p�[�c�q�b�g�G�t�F�N�g
	void BossPartsHitEffect();

	void PlayerHitEffect();

	void ReflectHitEffect();

	// �S�ẴR�A��j�󂵂���̏���
	void CoreAllBreak();

private: // �����o�ϐ�
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();
	DebugText debugText;

	Camera* camera = nullptr;

	ParticleManager* circleParticle = nullptr;

	Sprite* secondStageBG = nullptr;
	Sprite* Reticle = nullptr;
	Sprite* StoragePos = nullptr;

	Sprite* Black = nullptr;
	Sprite* Rule = nullptr;

	GageUI* gageBoost = nullptr;
	DeltaGageUI* gageBossHp = nullptr;
	DeltaGageUI* gagePlayerHp = nullptr;

	MeterUI* meterSpeed = nullptr;

	GageUI* gageCharge = nullptr;

	ProcessUI* processMainCore = nullptr;

	ObjModel* modelSkydome = nullptr;
	ObjModel* modelBullet = nullptr;
	ObjModel* modelBossPartsRing = nullptr;
	ObjModel* modelBossPartsCoreBox = nullptr;
	ObjModel* modelBossPartsSphere = nullptr;
	ObjModel* modelBossPartsTurretStand = nullptr;
	ObjModel* modelBossPartsBody = nullptr;
	ObjModel* modelBossPartsCoreStand = nullptr;

	Player* player = nullptr;
	Boss* boss = nullptr;

	BossParts* bossPartsCoreStand = nullptr;
	BossParts* bossPartsRing = nullptr;
	BossParts* bossPartsSphere = nullptr;
	BossParts* bossPartsBody = nullptr;

	BossMainCore* bossMainCore = nullptr;

	BossParts* bossTurretStand_1 = nullptr;

	BossTurret* bossTurret_1 = nullptr;

	BossShield* bossShield = nullptr;

	ObjObject* objSkydome = nullptr;

	std::list<std::unique_ptr<TargetBullet>> playerBullets;

	std::list<std::unique_ptr<Bullet>>bossTargetBullets;

	std::list<std::unique_ptr<ReflectBullet>>bossReflectBullets;

	//�I�u�W�F�N�g�p�[�e�B�N��
	std::list<std::unique_ptr<ObjectParticle>> particle;

	float shotRate = 0.0f;
	bool shotFlag = true;

	//���w�i�̕ϐ�
	bool BlackFlag = false;
	float BlackAlpha = 1.0f;

	float timer = 0.0f;

	//���e�B�N�����W
	XMFLOAT2 ReticlePos = { 0.0f, 0.0f };

	//�}�E�X���W
	POINT mousePosition;

	// ���C���R�A��HP�o�[�̍��W
	XMFLOAT2 bossHpUIPosition = { 1272.0f , 30.0f };

	// �u�[�X�g�Q�[�W�̍��W
	XMFLOAT2 boostUIPosition = { 538.0f, 695.0f };

	// �v���C���[�̑��x�Q�[�W�̍��W
	XMFLOAT2 playerSpeedUIPosition = { 640.0f, 690.0f };

	XMFLOAT2 playerHpUIPosition = { 1272.0f, 695.0f };

	XMFLOAT2 playerChargeUIPosition = { 1272.0f, 650.0f };

	float chargeNow = 0.0f;
	float chargeMax = 30.0f;
	float chargeRatio = 0.0f;
	float chargeSize = 0.0f;
	bool isCharge = false;

	int playerBulletType = Normal;

	XMFLOAT3 PlayerPos = { 0.0f, 0.0f, 0.0 };

	int turnCount = 1;
};
