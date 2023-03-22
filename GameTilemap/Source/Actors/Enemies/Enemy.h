#pragma once

class Player;

      enum class k_EnemyTypes { Default, };

class Enemy : public fw::GameObject
{

public:

    Enemy::Enemy(fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, Player* _pplayer, fw::GameCore* _pgame);
    ~Enemy();

    virtual void Update(float deltaTime);
    virtual void OnCollisionHit(GameObject* _pOtherObject);
    virtual void Draw() override;

    virtual void Die();

    virtual void TakeDamage(fw::GameObject* instigator, int damage);
    virtual void TakeDamage(int damage);

    int GetHealth();
    void SetHealth(int health);

    bool IsDead();

    virtual fw::k_ObjectType GetType() override;
    virtual fw::Collider* GetCollider() override;

protected:
    bool m_IsDead = false;
    Player* m_pPlayer = nullptr;
    fw::Collider* m_pCollider = nullptr;
    bool m_DrawDebugCollider = fw::k_Debug_DrawColliders;
    int m_Health = k_Shade_MaxHealth;

};