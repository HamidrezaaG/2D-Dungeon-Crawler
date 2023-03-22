#pragma once
#include "Enemy.h"

class Pathfinder;
class TileMap;

class Enemy_Shade : public Enemy
{
public:

	enum class k_Animation { Idle, Attack };
	std::string k_AnimationNames[2] = { "Idle", "Attack" };

	enum class k_Direction { Up, Down, Left, Right };
	std::string k_DirectionNames[4] = { "Up", "Down", "Left", "Right" };

	enum class k_State { Idle, Moving, Attacking, Hurting, Dying };
	enum class k_Behavior { Hunt, Wander };

	Enemy_Shade(IVector2 _gridposition, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Pathfinder* _pPathfinder, Player* _pplayer, fw::GameCore* _pgame);
	
	virtual void Die() override;
	virtual void TakeDamage(fw::GameObject* instigator, int damage) override;
	
	~Enemy_Shade();

	void SetBehavior(k_Behavior b);

	void Update(float deltaTime) override;

	void Attack();


	void GetRandomPath();
	bool Enemy_Shade::TryGetPathTo(IVector2 _targetPos);

	void SetSpriteSheet(fw::SpriteSheet* _pspritesheet);
	void SetSprite(fw::SpriteSheet::SpriteInfo* _spriteinfo);

	void UpdateAnimation(float _deltaTime);
	void IncrementStep();

    virtual fw::Collider* GetCollider() override;

private:

	k_Animation m_ECurrentAnimation= k_Animation::Idle;
	k_Direction m_ECurrentDirection= k_Direction::Down;
	k_State		m_EState		   = k_State::Idle;
	k_Behavior  m_ECurrentBehavior = k_Behavior::Wander;

	Vector2 m_CurrentPosition = Vector2::Zero();;;
	Vector2 m_Acceleration = Vector2::Zero();;

	IVector2 m_ThisStep = IVector2::Zero();
	IVector2 m_NextStep = IVector2::Zero();
	IVector2 m_FinalStep = IVector2::Zero();

	int m_Anim_FrameNumber = 0;
	float m_Anim_FrameTimer = 0.f;

	float m_Anim_FrameRate = 10;
	float m_Anim_AttackFrameRate = 20;
	int m_Anim_IdleFrameCount = 12;
	int m_Anim_IdleFrameRate = 12;

	float m_MovementTimer = 0.f;
	float m_MovementSpeed = 1.0f;

	float m_HuntRange = 6;
	float m_AttackRange = 0.5F;
	float m_DistanceToPlayer = 900.0f;
	
	float m_DeathTimer = 0;
	float m_HurtTimer = 0;

	int m_ShadeDamage = 1;
	float m_InvincibilityTimer = 0;

	bool m_CanMove = true;
	int path[128]{};
	
	std::vector<IVector2> m_PathList;

	fw::SpriteSheet* m_pSpriteSheet = nullptr;
	Pathfinder* m_pPathfinder = nullptr;
	TileMap* m_pTileMap = nullptr;
};
