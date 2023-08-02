#include "ThirdStage.h"

using namespace DirectX;
extern HWND hwnd;

ThirdStage::ThirdStage()
{
}

ThirdStage::~ThirdStage()
{
}

void ThirdStage::Initialize()
{
	// �T�E���h������
	sound->Initialize();

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// �J�����Z�b�g
	ObjObject::SetCamera(camera);
	Player::SetCamera(camera);
	Boss::SetCamera(camera);

	if (!Sprite::LoadTexture(TextureNumber::game_bg, L"Resources/Sprite/GameUI/game_bg.png")) {
		assert(0);
		return;
	}

	//���e�B�N���e�N�X�`��
	if (!Sprite::LoadTexture(TextureNumber::reticle, L"Resources/Re.png")) {
		assert(0);
		return;
	}

	// �{�X
	if (!Sprite::LoadTexture(TextureNumber::game_boss_frame_1, L"Resources/Sprite/GameUI/BossHpUI/game_boss_frame_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_boss_frame_2, L"Resources/Sprite/GameUI/BossHpUI/game_boss_frame_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_boss_gauge, L"Resources/Sprite/GameUI/BossHpUI/game_boss_gauge.png")) {
		assert(0);
		return;
	}

	//���w�i
	if (!Sprite::LoadTexture(TextureNumber::black, L"Resources/Sprite/TitleUI/Black.png")) {
		assert(0);
		return;
	}

	//���w�i
	if (!Sprite::LoadTexture(TextureNumber::rule, L"Resources/Sprite/TitleUI/Rule.png")) {
		assert(0);
		return;
	}

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// �f�o�b�O�e�L�X�g������
	debugText.Initialize(0);

	thirdStageBG = Sprite::Create(TextureNumber::game_bg, { 0.0f,0.0f });

	Reticle = Sprite::Create(TextureNumber::reticle, ReticlePos);

	StoragePos = Sprite::Create(TextureNumber::reticle, { (float)mousePosition.x, (float)mousePosition.y });

	// �u�[�X�g�Q�[�W
	gageBoost = GageUI::Create(boostUIPosition, { 530.0f, 30.0f }, { 0.6f, 0.6f, 0.1f, 1.0f });
	gageBossHp = DeltaGageUI::Create(bossHpUIPosition, { 530.0f, 30.0f });
	gagePlayerHp = DeltaGageUI::Create(playerHpUIPosition, { 530.0f, 30.0f });
	gageCharge = GageUI::Create(playerChargeUIPosition, { 530.0f, 30.0f }, { 0.6f, 0.1f, 0.1f, 1.0f });

	// ���x�Q�[�W
	gageSpeed = GageUI::Create(playerSpeedUIPosition, { 530.0f, 30.0f }, { 0.1f, 0.6f, 0.6f, 1.0f });

	Black = Sprite::Create(TextureNumber::black, { 0.0f, 0.0f });

	BlackAlpha = 1.0f;
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	Rule = Sprite::Create(TextureNumber::rule, { 0.0f, 0.0f });

	// �p�[�e�B�N��
	circleParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");

	// �v���C���[
	player = Player::Create();

	// �X�J�C�h�[��
	objSkydome = ObjObject::Create();

	// �{�X
	boss = Boss::Create();
	bossPartsRing = BossParts::Create();
	bossPartsSphere = BossParts::Create();
	bossCore_1 = BossCore::Create();
	bossCore_2 = BossCore::Create();
	bossCore_3 = BossCore::Create();
	bossCore_4 = BossCore::Create();
	bossCoreBox_1 = BossParts::Create();
	bossCoreBox_2 = BossParts::Create();
	bossCoreBox_3 = BossParts::Create();
	bossCoreBox_4 = BossParts::Create();
	bossTurretStand_1 = BossParts::Create();
	//bossTurretStand_2 = BossParts::Create();
	bossTurret_1 = BossTurret::Create();
	//bossTurret_2 = BossTurret::Create();
	bossPartsCoreStand = BossParts::Create();
	bossMainCore = BossMainCore::Create();

	// ���f���Z�b�g
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	objSkydome->SetModel(modelSkydome);

	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	modelBossPartsCoreBox = ObjModel::CreateFromOBJ("bossPartsCoreBox");
	bossCoreBox_1->SetModel(modelBossPartsCoreBox);
	bossCoreBox_2->SetModel(modelBossPartsCoreBox);
	bossCoreBox_3->SetModel(modelBossPartsCoreBox);
	bossCoreBox_4->SetModel(modelBossPartsCoreBox);

	modelBossPartsRing = ObjModel::CreateFromOBJ("bossPartsRing");
	bossPartsRing->SetModel(modelBossPartsRing);

	modelBossPartsSphere = ObjModel::CreateFromOBJ("bossPartsSphere");
	bossPartsSphere->SetModel(modelBossPartsSphere);

	modelBossPartsTurretStand = ObjModel::CreateFromOBJ("TurretStand");
	bossTurretStand_1->SetModel(modelBossPartsTurretStand);
	//bossTurretStand_2->SetModel(modelBossPartsTurretStand);

	modelBossPartsCoreStand = ObjModel::CreateFromOBJ("bossPartsCoreStand");
	bossPartsCoreStand->SetModel(modelBossPartsCoreStand);


	// ���W�̃Z�b�g
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 30, -10 });
	camera->SetUp({ 0, 1, 0 });
	camera->SetDistance(20.0f);

	// �v���C���[
	player->SetPosition({ 0.0f, 0.0f, -200.0f });
	player->SetRotation({ 0.0f, 180.0f, 0.0f });
	player->SetScale({ 2.0f, 2.0f, 2.0f });

	// �{�X�̃x�[�X
	boss->SetPosition({ 0.0f,0.0f,0.0f });
	boss->SetRotation({ 0.0f, 0.0f, 0.0f });
	boss->SetScale({ 7.0f, 7.0f, 7.0f });

	// �{�X�̃����O�p�[�c
	bossPartsRing->SetScale({ 4.0f, 4.0f, 4.0f });
	bossPartsRing->SetParent(boss);

	// �{�X�̋��p�[�c
	bossPartsSphere->SetScale({ 4.2f, 4.2f, 4.2f });
	bossPartsSphere->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	bossPartsSphere->SetParent(boss);

	bossPartsCoreStand->SetPosition({ 0.0f, -3.5f, 0.0f });
	bossPartsCoreStand->SetRotation({ 180.0f, 0.0f, 0.0f });
	bossPartsCoreStand->SetScale({ 2.0f, 2.0f, 2.0f });
	bossPartsCoreStand->SetColor({ 0.647f, 0.619f, 0.658f, 1.0f });
	bossPartsCoreStand->SetParent(boss);

	// �{�X�̃R�A�{�b�N�X1
	bossCoreBox_1->SetPosition({ 0.0f , 0.0f, -1.2f });
	bossCoreBox_1->SetRotation({ 0.0f, 180.0f, 0.0f });
	bossCoreBox_1->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCoreBox_1->SetParent(bossPartsRing);

	// �{�X�̃R�A�{�b�N�X2
	bossCoreBox_2->SetPosition({ 1.2f , 0.0f, 0.0f });
	bossCoreBox_2->SetRotation({ 0.0f, 90.0f, 0.0f });
	bossCoreBox_2->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCoreBox_2->SetParent(bossPartsRing);

	// �{�X�̃R�A�{�b�N�X3
	bossCoreBox_3->SetPosition({ 0.0f , 0.0f, 1.2f });
	bossCoreBox_3->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCoreBox_3->SetParent(bossPartsRing);

	// �{�X�̃R�A�{�b�N�X4
	bossCoreBox_4->SetPosition({ -1.2f , 0.0f, 0.0f });
	bossCoreBox_4->SetRotation({ 0.0f, 270.0f, 0.0f });
	bossCoreBox_4->SetScale({ 0.3f, 0.3f, 0.3f });
	bossCoreBox_4->SetParent(bossPartsRing);

	// �{�X�̃R�A1
	bossCore_1->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_1->SetScale({ 0.7f, 0.7f, 0.7f });
	bossCore_1->SetParent(bossCoreBox_1);

	// �{�X�̃R�A2
	bossCore_2->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_2->SetScale({ 0.7f, 0.7f, 0.7f });
	bossCore_2->SetParent(bossCoreBox_2);

	// �{�X�̃R�A3
	bossCore_3->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_3->SetScale({ 0.7f, 0.7f, 0.7f });
	bossCore_3->SetParent(bossCoreBox_3);

	// �{�X�̃R�A4
	bossCore_4->SetPosition({ 0.0f , 0.0f, 1.0f });
	bossCore_4->SetScale({ 0.7f, 0.7f, 0.7f });
	bossCore_4->SetParent(bossCoreBox_4);

	// �{�X�̃��C���R�A
	bossMainCore->SetPosition({ 0.0f, 0.0f, 0.0f });
	bossMainCore->SetScale({ 0.75f, 0.75f, 0.75f });
	bossMainCore->SetParent(bossPartsCoreStand);

	// �{�X�̖C��1
	bossTurretStand_1->SetPosition({ 0.0f, 0.9f, 0.0f });
	bossTurretStand_1->SetScale({ 0.3f, 0.3f, 0.3f });
	bossTurretStand_1->SetParent({ bossPartsSphere });

	bossTurret_1->SetPosition({ 0.0f, 2.5, 0.0f });
	bossTurret_1->SetScale({ 1.0f, 1.0f, 1.0f });
	bossTurret_1->SetParent({ bossTurretStand_1 });

	// �{�X�̖C��2
	/*bossTurretStand_2->SetPosition({ 0.0f, -0.9f, 0.0f });
	bossTurretStand_2->SetRotation({ 0.0f, 0.0f, 180.0f });
	bossTurretStand_2->SetScale({ 0.3f, 0.3f, 0.3f });
	bossTurretStand_2->SetParent({ bossPartsSphere });

	bossTurret_2->SetPosition({ 0.0f, 2.5, 0.0f });
	bossTurret_2->SetScale({ 1.0f, 1.0f, 1.0f });
	bossTurret_2->SetParent({ bossTurretStand_2 });*/

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objSkydome->SetRotation({ 0.0f,0.0f,0.0f, });
	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });

	camera->SetTarget(boss->GetPosition());
	camera->SetEye({ 0, 0, -10 });
	camera->SetUp({ 0, 1, 0 });
	//camera->SetDistance(48.0f);

	ShowCursor(false);
}

void ThirdStage::Finalize()
{
}

void ThirdStage::Update()
{
	BlackAlpha -= 0.02f;
	Black->SetColor({ 1.0f, 1.0f, 1.0f, BlackAlpha });

	//RECT�\���̂ւ̃|�C���^
	RECT rect;

	//�E�B���h�E�̊O���̃T�C�Y���擾
	GetClientRect(hwnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	//�}�E�X��(�X�N���[��)���W���擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	ScreenToClient(hwnd, &mousePosition);

	//�}�E�X���W��2D���e�B�N���̃X�v���C�g�ɑ��
	ReticlePos.x = ((float)(mousePosition.x) / (float)width) * WinApp::window_width;
	ReticlePos.y = ((float)(mousePosition.y) / (float)height) * WinApp::window_height;

	if (ReticlePos.x <= 280.0f)
	{
		ReticlePos.x = 280.0f;
	}
	if (ReticlePos.x >= 1000.0f)
	{
		ReticlePos.x = 1000.0f;
	}

	if (ReticlePos.y <= 180.0f)
	{
		ReticlePos.y = 180.0f;
	}
	if (ReticlePos.y >= 575.0f)
	{
		ReticlePos.y = 575.0f;
	}

	// Debug Use Only
	if (input->TriggerKey(DIK_C))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}

	//circleParticle->SparkParticle(20, 50, bossCore_1->GetWorldPosition(), 10.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());

	// �R�A�q�b�g�G�t�F�N�g
	CoreHitEffect();

	// �{�X�p�[�c�q�b�g�G�t�F�N�g
	BossPartsHitEffect();

	// �v���C���[�̋����ˏ���
	if (playerBulletType == Normal)
	{
		Shoot();

		if (input->TriggerKey(DIK_T))
		{
			playerBulletType = Charge;
		}
	}
	else if (playerBulletType == Charge)
	{
		chargeShoot();

		if (input->TriggerKey(DIK_T))
		{
			playerBulletType = Normal;
		}
	}

	// �v���C���[�̑_���e������
	playerBullets.remove_if([](std::unique_ptr<TargetBullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// �{�X�̖C��1�����Ԋu�Ŕ���
	if (bossTurret_1->isAlive == true)
	{
		bossTurret_1->shotTimer--;
		if (bossTurret_1->shotTimer <= 0)
		{
			BossTargetShoot(bossTurret_1->GetWorldPosition(), player->GetPosition(), 10.0f);
			bossTurret_1->shotTimer = bossTurret_1->ShotInterval;
		}
	}

	// �{�X�̑_���e���X�V
	for (std::unique_ptr<Bullet>& bullet : bossTargetBullets)
	{
		circleParticle->BulletParticle(5, 10, bullet->GetPosition(), { 1.0f,0.1f, 0.1f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, 7.0f);
		bullet->Update();
	}

	// �{�X�̑_���e������
	bossTargetBullets.remove_if([](std::unique_ptr<Bullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	//�I�u�W�F�N�g�p�[�e�B�N�����X�V
	for (std::unique_ptr<ObjectParticle>& part : particle)
	{
		//part->Update();
	}

	particle.remove_if([](std::unique_ptr<ObjectParticle>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// �R�A���j�G�t�F�N�g
	CoreBreakEffect();

	// �S�ẴR�A��j�󂵂���
	CoreAllBreak();

	if (input->PushKey(DIK_H))
	{
		playerSpeedUIPosition.y += 1.0f;
	}

	PlayerHitEffect();

	// �u�[�X�g�Q�[�W
	gageBoost->Update(player->GetBoostPowNow(), player->GetBoostPowMax(), boostUIPosition, { 0.6f, 0.6f, 0.1f, 1.0f }, { 0.6f, 0.1f, 0.1f, 1.0f });
	// �{�X��HP�Q�[�W
	gageBossHp->Update(bossMainCore->life, bossMainCore->lifeMax, bossHpUIPosition);
	// �v���C���[�̑��x�Q�[�W
	gageSpeed->Update(player->GetTotalSpeed(), player->GetTotalSpeedMax(), playerSpeedUIPosition, { 0.1f, 0.6f, 0.6f, 1.0f }, { 0.1f, 0.6f, 0.6f, 1.0f });

	gagePlayerHp->Update(player->HP, player->HPMAX, playerHpUIPosition);

	gageCharge->Update(chargeNow, chargeMax, playerChargeUIPosition, { 0.1f, 0.6f, 0.1f, 1.0f }, { 0.6f, 0.1f, 0.1f, 1.0f });

	// �J�����^�[�Q�b�g�̃Z�b�g
	// camera->SetTarget(boss->GetPosition());
	// camera->SetDistance(sqrtf(pow(boss->GetPosition().x - player->GetPosition().x, 2) + pow(boss->GetPosition().y - player->GetPosition().y, 2) + pow(boss->GetPosition().z - player->GetPosition().z, 2)) + 48.0f);

	// �J�����̍X�V
	camera->Update(player->GetPosition(), boss->GetPosition());

	// �v���C���[�̍X�V
	player->Update();

	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		circleParticle->BulletParticle(5, 10, bullet->GetPosition(), { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 5.0f);
		bullet->eyePosition = camera->GetEye();
		bullet->targetPosition = camera->GetTarget();
		bullet->upVector = camera->GetUp();
		bullet->Update(player->prevPos, player->GetPosition(), camera->GetViewMatrix(), player->GetRotation());
	}

	boss->Update();
	bossPartsRing->Update();
	bossPartsSphere->Update();
	bossCoreBox_1->Update();
	bossCoreBox_2->Update();
	bossCoreBox_3->Update();
	bossCoreBox_4->Update();
	bossCore_1->Update();
	bossCore_2->Update();
	bossCore_3->Update();
	bossCore_4->Update();
	bossTurret_1->Update(player->GetPosition());
	//bossTurret_2->Update();
	bossTurretStand_1->SetRotation({ bossTurretStand_1->GetRotation().x, bossTurret_1->GetExternRotationY(), bossTurretStand_1->GetRotation().z });
	bossTurretStand_1->Update();
	//bossTurretStand_2->Update();
	bossPartsCoreStand->Update();
	bossMainCore->Update();

	PlayerMovementBoundaryChecking();

	objSkydome->Update();
	Reticle->SetAnchorPoint({ 0.5f, 0.5f });
	Reticle->SetPosition(ReticlePos);

	StoragePos->SetPosition({ (float)mousePosition.x, (float)mousePosition.y });

	DrawDebugText();

	circleParticle->JettParticle(5, 10, player->GetPosition(), player->GetVel(), 1.0f, 0.0f, { 0.941f, 0.231f, 0.156f, 1.0f }, { 0.941f, 0.862f, 0.156f, 1.0f });

	if (player->GetBoostFlag() == true)
	{
		circleParticle->BoostParticle(5, 50, player->GetPosition(), 1.0f, 0.0f, player->GetBoostPartColor(), player->GetBoostPartColor());
	}
	// �p�[�e�B�N���X�V
	circleParticle->Update();

	// HP��0�ɂȂ����猂�j
	if (bossMainCore->life <= 0.0f)
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}
}

void ThirdStage::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	thirdStageBG->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	ObjObject::PreDraw(cmdList);

	// 3D�I�u�N�W�F�N�g�̕`��
	player->Draw();
	boss->Draw();

	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		//bullet->Draw();
	}

	for (std::unique_ptr<Bullet>& bullet : bossTargetBullets)
	{
		//bullet->Draw();
	}

	bossPartsRing->Draw();
	bossPartsSphere->Draw();
	bossCoreBox_1->Draw();
	bossCoreBox_2->Draw();
	bossCoreBox_3->Draw();
	bossCoreBox_4->Draw();
	bossTurret_1->Draw();
	//bossTurret_2->Draw();
	bossTurretStand_1->Draw();
	//bossTurretStand_2->Draw();
	bossPartsCoreStand->Draw();

	bossCore_1->Draw();
	bossCore_2->Draw();
	bossCore_3->Draw();
	bossCore_4->Draw();

	bossMainCore->Draw();

#pragma region ���ʔj�󏈗����ꎞ�R�����g�A�E�g
	/*if (bossCore_1->isAlive || bossCore_2->isAlive || bossCore_3->isAlive || bossCore_4->isAlive)
	{
		bossPartsRing->Draw();
		bossPartsSphere->Draw();
		bossCoreBox_1->Draw();
		bossCoreBox_2->Draw();
		bossCoreBox_3->Draw();
		bossCoreBox_4->Draw();
		bossTurret_1->Draw();
		bossTurret_2->Draw();
		bossTurretStand_1->Draw();
		bossTurretStand_2->Draw();
	}

	if (bossCore_1->isAlive)
	{
		bossCore_1->Draw();
	}

	if (bossCore_2->isAlive)
	{
		bossCore_2->Draw();
	}

	if (bossCore_3->isAlive)
	{
		bossCore_3->Draw();
	}

	if (bossCore_4->isAlive)
	{
		bossCore_4->Draw();
	}*/
#pragma endregion

	objSkydome->Draw();

	// �p�[�e�B�N���̕`��
	circleParticle->Draw(cmdList);

	// 3D�I�u�W�F�N�g�`��㏈��
	ObjObject::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �O�i�X�v���C�g�`��
	Reticle->Draw();

	gageBoost->Draw();
	gageBossHp->Draw();
	gageSpeed->Draw();
	gagePlayerHp->Draw();
	gageCharge->Draw();

	player->DebugTextDraw();
	debugText.DrawAll(cmdList);
	//Rule->Draw();
	Black->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}

// �}�E�X�̏���
void ThirdStage::GetMouse()
{
	//�}�E�X��(�X�N���[��)���W���擾����
	GetCursorPos(&mousePosition);

	//�N���C�A���g�G���A���W�ɕϊ�����
	HWND hwnd = WinApp::GetInst()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//�}�E�X���W��2D���e�B�N���̃X�v���C�g�ɑ��
	ReticlePos.x = (float)mousePosition.x;
	ReticlePos.y = (float)mousePosition.y;
}

void ThirdStage::DrawDebugText()
{
	std::ostringstream ThirdStage;
	ThirdStage << "ThirdStage";
	debugText.Print(ThirdStage.str(), 10, 10, 2.0f);
}

// �v���C���[�e����
void ThirdStage::Shoot()
{
	shotRate -= 0.1f;

	PlayerPos = player->GetPosition();

	XMVECTOR bulletVelocity = { 0,0,1.0f };

	player->shootSpeed = Lerp::LerpFloat(player->GetShootSpeedMin(), player->GetNormalShootSpeedMax(), player->shootSpeedTimeRate);

	if (Input::GetInstance()->PushKey(DIK_SPACE) || Input::GetInstance()->PushMouseLeft())
	{
		if (shotRate <= 0)
		{
			shotFlag = true;
		}

		if (shotFlag == true)
		{
			std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
			newBullet = TargetBullet::Create(modelBullet, { player->GetPosition().x, player->GetPosition().y + 0.3f, player->GetPosition().z }, { 1.0f, 1.0f, 1.0f }, boss->GetPosition(), 30.0f, camera->GetEye(), camera->GetTarget(), camera->GetUp(), player->GetPosition());
			newBullet->eyePosition = camera->GetEye();
			//newBullet->eyePosition.y += 10.0f;
			newBullet->targetPosition = camera->GetTarget();
			newBullet->upVector = camera->GetUp();
			playerBullets.push_back(std::move(newBullet));

			shotFlag = false;
			shotRate = 1.5f;
		}

		player->shootSpeedTimeRate += 0.05f;
	}
	else if (!Input::GetInstance()->PushMouseLeft())
	{
		player->shootSpeedTimeRate -= 0.1f;
	}

	player->shootSpeed = max(player->shootSpeed, player->GetShootSpeedMin());
	player->shootSpeed = min(player->shootSpeed, player->GetNormalShootSpeedMax());

	player->shootSpeedTimeRate = max(player->shootSpeedTimeRate, 0.0f);
	player->shootSpeedTimeRate = min(player->shootSpeedTimeRate, 1.0f);
}

void ThirdStage::chargeShoot()
{
	chargeRatio = chargeNow / chargeMax;

	chargeSize = Easing::InOutQuadFloat(0.0, 10.0, chargeRatio);

	circleParticle->BulletParticle(5, 2, player->GetPosition(), { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, chargeSize);

	player->shootSpeed = Lerp::LerpFloat(player->GetShootSpeedMin(), player->GetChargeShootSpeedMax(), player->shootSpeedTimeRate);

	if (isCharge == false)
	{
		if (chargeNow < chargeMax)
		{
			if (Input::GetInstance()->PushMouseLeft())
			{
				chargeNow++;
				player->shootSpeedTimeRate += 0.05f;
			}
			else if (!Input::GetInstance()->PushMouseLeft())
			{
				chargeNow -= 2.0f;
				player->shootSpeedTimeRate -= 0.1f;
			}
		}
		else
		{
			if (!Input::GetInstance()->PushMouseLeft())
			{
				isCharge = true;
			}
		}
	}
	else
	{
		std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
		newBullet = TargetBullet::Create(modelBullet, { player->GetPosition().x, player->GetPosition().y + 0.3f, player->GetPosition().z }, { 1.0f, 1.0f, 1.0f }, boss->GetPosition(), 15.0f, camera->GetEye(), camera->GetTarget(), camera->GetUp(), player->GetPosition());
		newBullet->eyePosition = camera->GetEye();
		//newBullet->eyePosition.y += 10.0f;
		newBullet->targetPosition = camera->GetTarget();
		newBullet->upVector = camera->GetUp();
		playerBullets.push_back(std::move(newBullet));

		chargeNow = 0.0f;
		isCharge = false;
	}

	player->shootSpeed = max(player->shootSpeed, player->GetShootSpeedMin());
	player->shootSpeed = min(player->shootSpeed, player->GetChargeShootSpeedMax());

	player->shootSpeedTimeRate = max(player->shootSpeedTimeRate, 0.0f);
	player->shootSpeedTimeRate = min(player->shootSpeedTimeRate, 1.0f);

	chargeNow = max(chargeNow, 0.0);
	chargeNow = min(chargeNow, chargeMax);
}

void ThirdStage::PlayerMovementBoundaryChecking()
{
	if (player->CheckCollisionWithBoss(boss->GetPosition(), 80.0f))
	{
		float dx = player->GetPosition().x - boss->GetPosition().x;
		float dy = player->GetPosition().y - boss->GetPosition().y;
		float dz = player->GetPosition().z - boss->GetPosition().z;

		float length = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));

		if (length > 0.0f)
		{
			dx /= length;
			dy /= length;
			dz /= length;
		}

		float newX = player->GetPosition().x;
		float newY = player->GetPosition().y;
		float newZ = player->GetPosition().z;

		float elapsedTime = 0.0f;

		while (player->CheckCollisionWithBoss(boss->GetPosition(), 80.0f))
		{
			player->MoveTowards(newX, player->GetPosition().x + dx, 1.0f, elapsedTime);
			player->MoveTowards(newY, player->GetPosition().y + dy, 1.0f, elapsedTime);
			player->MoveTowards(newZ, player->GetPosition().z + dz, 1.0f, elapsedTime);
			player->SetPosition({ newX, newY, newZ });
			elapsedTime += 0.1f;
		}
	}
	if (!player->CheckCollisionWithBoss(boss->GetPosition(), 440.0f))
	{
		float dx = player->GetPosition().x - boss->GetPosition().x;
		float dy = player->GetPosition().y - boss->GetPosition().y;
		float dz = player->GetPosition().z - boss->GetPosition().z;

		float length = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));

		if (length > 0.0f)
		{
			dx /= length;
			dy /= length;
			dz /= length;
		}

		float newX = player->GetPosition().x;
		float newY = player->GetPosition().y;
		float newZ = player->GetPosition().z;

		float elapsedTime = 0.0f;

		while (!player->CheckCollisionWithBoss(boss->GetPosition(), 440.0f))
		{
			player->MoveTowards(newX, player->GetPosition().x - dx, 1.0f, elapsedTime);
			player->MoveTowards(newY, player->GetPosition().y - dy, 1.0f, elapsedTime);
			player->MoveTowards(newZ, player->GetPosition().z - dz, 1.0f, elapsedTime);
			player->SetPosition({ newX, newY, newZ });
			elapsedTime += 0.1f;
		}
	}
}

void ThirdStage::BossTargetShoot(XMFLOAT3 startPosition, XMFLOAT3 endPosition, float bulletSpeed)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet = Bullet::Create(modelBullet, startPosition, { 1.0f, 1.0f, 1.0f }, endPosition, bulletSpeed);

	bossTargetBullets.push_back(std::move(newBullet));
}

// �R�A���j�G�t�F�N�g
void ThirdStage::CoreBreakEffect()
{
	//���C�t��0�ɂȂ������ɃI�u�W�F�N�g�̈ʒu���猂�j�p�[�e�B�N���𔭐�
	// �R�A1
	if (bossCore_1->GetAliveFlag() == false)
	{
		if (bossCore_1->GetDestroyPartTime() > 0)
		{
			circleParticle->BlastPart_1(20, bossCore_1->GetWorldPosition(), 20.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());
		}
	}

	// �R�A2
	if (bossCore_2->GetAliveFlag() == false)
	{
		if (bossCore_2->GetDestroyPartTime() > 0)
		{
			circleParticle->BlastPart_2(20, bossCore_2->GetWorldPosition(), 20.0f, 0.0f, bossCore_2->GetColorRed(), bossCore_2->GetColorRed());
		}
	}

	// �R�A3
	if (bossCore_3->GetAliveFlag() == false)
	{
		if (bossCore_3->GetDestroyPartTime() > 0)
		{
			circleParticle->BlastPart_3(20, bossCore_3->GetWorldPosition(), 20.0f, 0.0f, bossCore_3->GetColorRed(), bossCore_3->GetColorRed());
		}
	}

	// �R�A4
	if (bossCore_4->GetAliveFlag() == false)
	{
		if (bossCore_4->GetDestroyPartTime() > 0)
		{
			circleParticle->BlastPart_4(20, bossCore_4->GetWorldPosition(), 20.0f, 0.0f, bossCore_4->GetColorRed(), bossCore_4->GetColorRed());
		}
	}
}

// �R�A�q�b�g�G�t�F�N�g
void ThirdStage::CoreHitEffect()
{
	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		// �R�A1�̋^���q�b�g����
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossCore_1->GetWorldPosition(), 24.0f))
		{
			if (bossCore_1->GetLife() <= 1 && bossCore_1->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossCore_1->GetWorldPosition(), 50.0f, 0.0f, bossCore_1->GetColorRed(), bossCore_1->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossCore_1->GetWorldPosition(), 25.0f, 0.0f, bossCore_1->GetColorYellow(), bossCore_1->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossCore_1->GetWorldPosition(), 25.0f, 0.0f, bossCore_1->GetColorOrange(), bossCore_1->GetColorOrange());
			}
			bossCore_1->colorTimeRate = 0.0f;
			bossCore_1->colorTimeRate2 = 0.0f;
			bossCore_1->life--;
			bullet->deathFlag = true;
		}

		// �R�A2�̋^���q�b�g����
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossCore_2->GetWorldPosition(), 24.0f))
		{
			if (bossCore_2->GetLife() <= 1 && bossCore_2->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossCore_2->GetWorldPosition(), 50.0f, 0.0f, bossCore_2->GetColorRed(), bossCore_2->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossCore_2->GetWorldPosition(), 25.0f, 0.0f, bossCore_2->GetColorYellow(), bossCore_2->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossCore_2->GetWorldPosition(), 25.0f, 0.0f, bossCore_2->GetColorOrange(), bossCore_2->GetColorOrange());
			}
			bossCore_2->colorTimeRate = 0.0f;
			bossCore_2->colorTimeRate2 = 0.0f;
			bossCore_2->life--;
			bullet->deathFlag = true;
		}

		// �R�A3�̋^���q�b�g����
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossCore_3->GetWorldPosition(), 24.0f))
		{
			if (bossCore_3->GetLife() <= 1 && bossCore_3->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossCore_3->GetWorldPosition(), 50.0f, 0.0f, bossCore_3->GetColorRed(), bossCore_3->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossCore_3->GetWorldPosition(), 25.0f, 0.0f, bossCore_3->GetColorYellow(), bossCore_3->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossCore_3->GetWorldPosition(), 25.0f, 0.0f, bossCore_3->GetColorOrange(), bossCore_3->GetColorOrange());
			}
			bossCore_3->colorTimeRate = 0.0f;
			bossCore_3->colorTimeRate2 = 0.0f;
			bossCore_3->life--;
			bullet->deathFlag = true;
		}

		// �R�A4�̋^���q�b�g����
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossCore_4->GetWorldPosition(), 24.0f))
		{
			if (bossCore_4->GetLife() <= 1 && bossCore_4->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossCore_4->GetWorldPosition(), 50.0f, 0.0f, bossCore_4->GetColorRed(), bossCore_4->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossCore_4->GetWorldPosition(), 25.0f, 0.0f, bossCore_4->GetColorYellow(), bossCore_4->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossCore_4->GetWorldPosition(), 25.0f, 0.0f, bossCore_4->GetColorOrange(), bossCore_4->GetColorOrange());
			}
			bossCore_4->colorTimeRate = 0.0f;
			bossCore_4->colorTimeRate2 = 0.0f;
			bossCore_4->life--;
			bullet->deathFlag = true;
		}

		// ���C���R�A�̋^���q�b�g����
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossMainCore->GetWorldPosition(), 24.0f))
		{
			if (bossMainCore->GetLife() <= 1 && bossMainCore->GetLife() >= 0)
			{
				circleParticle->DefaultParticle(20, 50, bossMainCore->GetWorldPosition(), 50.0f, 0.0f, bossMainCore->GetColorRed(), bossMainCore->GetColorRed());
				circleParticle->DefaultParticle(10, 50, bossMainCore->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorYellow(), bossMainCore->GetColorYellow());
				circleParticle->DefaultParticle(10, 50, bossMainCore->GetWorldPosition(), 25.0f, 0.0f, bossMainCore->GetColorOrange(), bossMainCore->GetColorOrange());
			}

			if (bossMainCore->isAlive == true)
			{
				bossMainCore->life--;
			}

			bossMainCore->colorTimeRate = 0.0f;
			bossMainCore->colorTimeRate2 = 0.0f;
			bullet->deathFlag = true;
		}
	}
}

// �{�X�p�[�c�q�b�g�G�t�F�N�g
void ThirdStage::BossPartsHitEffect()
{
	for (std::unique_ptr<TargetBullet>& bullet : playerBullets)
	{
		// Turret 1 collision detection
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, bossTurret_1->GetWorldPosition(), 24.0f))
		{
			// �K�v�ȂƂ��͂��ł��A����3�s�����R�ɕ������Ă��������B
			if (bossTurret_1->isAlive == true)
			{
				bossTurret_1->colorTimeRate = 0.0f;
				bossTurret_1->life--;
			}
			bullet->deathFlag = true;
		}

		// Turret 2 collision detection
		//if (BasicCollisionDetection(bullet->GetPosition(), 3.0f, bossTurret_2->GetWorldPosition(), 24.0f))
		//{
		//	// �K�v�ȂƂ��͂��ł��A����3�s�����R�ɕ������Ă��������B
		//	if (bossTurret_2->isAlive == true)
		//	{
		//		bossTurret_2->colorTimeRate = 0.0f;
		//		bossTurret_2->life--;
		//	}
		//	bullet->deathFlag = true;
		//}
	}
}

void ThirdStage::PlayerHitEffect()
{
	for (std::unique_ptr<Bullet>& bullet : bossTargetBullets)
	{
		if (Collision::CCDCollisionDetection(bullet->prevPosition, bullet->GetPosition(), 3.0f, player->GetPosition(), 8.0f))
		{
			player->HP -= 1.0f;
			circleParticle->DefaultParticle(20, 50, player->GetPosition(), 20.0f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

			bullet->deathFlag = true;
		}
	}

	if (player->HP <= 0.0f)
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}
}

void ThirdStage::CoreAllBreak()
{
	if (bossMainCore->isBreak == true)
	{
		bossCore_1->VarReset();
		bossCore_2->VarReset();
		bossCore_3->VarReset();
		bossCore_4->VarReset();

		bossMainCore->VarReset();

		if (bossCore_1->isAlive && bossCore_2->isAlive && bossCore_3->isAlive && bossCore_4->isAlive && bossMainCore->isAlive == false)
		{
			bossMainCore->isBreak = false;
		}
	}

	if (bossMainCore->isAlive == false)
	{
		if (!bossCore_1->isAlive && !bossCore_2->isAlive && !bossCore_3->isAlive && !bossCore_4->isAlive)
		{
			bossMainCore->isAlive = true;
		}
	}
}