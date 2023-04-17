#pragma once

#include "CollisionPrimitive.h"
#include "RaycastHit.h"

#include <d3d12.h>
#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:// �ÓI�����o�֐�
	static CollisionManager* GetInstance();

public:// �����o�֐�
	// �R���C�_�[�̒ǉ�
	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}

	// �R���C�_�[�̍폜
	inline void RemoveCollider(BaseCollider* collider) {
		colliders.remove(collider);
	}

	// �S�Ă̏Փ˃`�F�b�N
	void CheckAllCollisions();

	// ���C�L���X�g
	bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;

	// �R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*> colliders;
};