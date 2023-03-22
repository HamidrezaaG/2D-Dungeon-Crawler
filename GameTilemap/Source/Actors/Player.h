#pragma once

class PlayerController;

class Player : public fw::GameObject
{

public:

    enum class k_Animation { Idle, Run, Attack };
    std::string k_AnimationNames[3] = { "Idle", "Run", "Attack" };

    enum class k_Direction { Up, Down, Left, Right };
    std::string k_DirectionNames[4] = { "Up", "Down", "Left", "Right" };

    enum class k_Player_State { Default, Dead };


    Player::Player(Vector2 position, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, PlayerController* _pcontroller, fw::GameCore* _pgame);
    ~Player();

    void SetSpriteSheet(fw::SpriteSheet* _pspritesheet);

    void HandleMovement(float deltaTime);

    void Update(float deltaTime) override;
    virtual fw::k_ObjectType GetType() override;
    virtual void Draw() override;

    void TakeDamage(int damage);
    void TakeDamage(fw::GameObject* instigator, int damage);

    void OnTileCollision(fw::RectangleCollider* _tileCollider);

    void SetState(k_Player_State state);

    fw::Collider* GetCollider() override;
    void UpdateAnimation(float _deltaTime);
    void SetSprite(fw::SpriteSheet::SpriteInfo* _spriteinfo);
    void Reset();

    void OnCollisionHit(GameObject* _pOtherObject) override;

    bool IsInvincible();

protected:

    Vector2 GetDirectionVector();
    k_Player_State m_CurrentState = k_Player_State::Default;

    fw::FWCore* m_pFramework = nullptr;
    PlayerController* m_pController = nullptr;
    Vector2 m_Acceleration;

    fw::Collider* m_pCollider;
    fw::SpriteSheet* m_pSpriteSheet;

    k_Animation m_CurrentAnimation;
    k_Direction m_CurrentDirection;

    int m_AnimationFrameNumber = 0;
    float m_AnimationFrameTimer = 0.f;

    int m_RunFrameRate = 0;
    bool m_CanMove = true;

    bool m_DrawDebugCollider = fw::k_Debug_DrawColliders;
    float colorAnim = 0;

    float m_InvincibilityTimer = 0;
    int m_Health = k_Player_MaxHealth;
};