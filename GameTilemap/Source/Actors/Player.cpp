#include "GamePCH.h"
#include "Player.h"
#include "PlayerController.h"
#include "../Events/GameEvents.h"

Player::Player(Vector2 position, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, PlayerController* _pcontroller, fw::GameCore* _pgame) : GameObject(position, _pmesh, _name, _pshader, _pgame),
m_pSpriteSheet(nullptr)
{
    m_pFramework = GameObject::m_pGameCore->GetFramework();
    m_pController = _pcontroller;

    //m_pCollider = new fw::RectangleCollider(this, Vector2(1,0.5f), Vector2(0,0), m_pActiveCamera, _pgame->GetDebugShader(), true);
    m_pCollider = new fw::CircleCollider(this, 0.35f, m_pActiveCamera, _pgame->GetDebugShader(), Vector2(0), true);
    m_pCollider->Enable();

    m_CurrentAnimation = k_Animation::Idle;
    m_CurrentDirection = k_Direction::Down;
}

Player::~Player()
{
    delete m_pCollider;
}

void Player::SetSpriteSheet(fw::SpriteSheet* _pspritesheet)
{
    m_pSpriteSheet = _pspritesheet;
}

void Player::HandleMovement(float deltaTime)
{

    m_Acceleration.Set(0, 0);

    if (m_CanMove)
    {
        if (m_pController->IsHeld(PlayerController::Mask::Up))
        {
            m_Acceleration.y += k_Player_Acceleration;
            m_CurrentDirection = k_Direction::Up;
        }
        if (m_pController->IsHeld(PlayerController::Mask::Down))
        {
            m_Acceleration.y -= k_Player_Acceleration;
            m_CurrentDirection = k_Direction::Down;
        }
        if (m_pController->IsHeld(PlayerController::Mask::Left))
        {
            m_Acceleration.x -= k_Player_Acceleration;
            m_CurrentDirection = k_Direction::Left;
        }
        if (m_pController->IsHeld(PlayerController::Mask::Right))
        {
            m_Acceleration.x += k_Player_Acceleration;
            m_CurrentDirection = k_Direction::Right;
        }
    }

    //if (abs(m_Velocity.x) > abs(m_Velocity.y)) {
    //    m_CurrentDirection = m_Velocity.x > 0 ? k_Direction::Right : k_Direction::Left;
    //}
    //else {
    //    m_CurrentDirection = m_Velocity.y > 0 ? k_Direction::Up : k_Direction::Down;
    //}

    if (m_pController->WasNewlyPressed(PlayerController::Mask::Attack))
    {
        if (m_CurrentAnimation != k_Animation::Attack)
        {
            m_AnimationFrameNumber = 0;
            m_AnimationFrameTimer = 0;
        }

        m_CurrentAnimation = k_Animation::Attack;
        m_CanMove = false;
        m_Velocity.Set(0, 0);
    }

    m_Velocity += m_Acceleration * deltaTime;
    m_Velocity = Vector2::Lerp(m_Velocity, Vector2::Zero(), deltaTime / k_Player_FrictionCoefficient);
    m_Velocity.ClampLength(k_Player_MaxSpeed);

    if (m_CurrentAnimation != k_Animation::Attack) {
        if (m_Velocity.SqrMagnitude() > k_Player_Anim_VelocityIdleThreshold)
        {
            m_CurrentAnimation = k_Animation::Run;
        }
        else if (m_Acceleration.IsZero()) {
            m_CurrentAnimation = k_Animation::Idle;

        }
    }
    m_Position += m_Velocity * deltaTime;
}

void Player::Update(float deltaTime)
{

    switch (m_CurrentState)
    {
    case Player::k_Player_State::Default:

        colorAnim += deltaTime;

        if (colorAnim > 2 * (float)fw::k_PI)
            colorAnim -= 2 * (float)fw::k_PI;

        if (m_InvincibilityTimer > 0)
        {
            sin(m_InvincibilityTimer * 60) > 0 ? SetColor(Color(sinf(colorAnim) * 0.25f + 0.75f, sinf(colorAnim) * 0.1f + 0.9f, 1, 1))
            : SetColor(Color(sinf(colorAnim) * 0.25f + 0.75f, sinf(colorAnim) * 0.1f + 0.9f, 1, 0.3f));
            m_InvincibilityTimer -= deltaTime;
        }
        else {
            SetColor(Color(sinf(colorAnim) * 0.25f + 0.75f, sinf(colorAnim) * 0.1f + 0.9f, 1, 1));
        }
        HandleMovement(deltaTime);
        UpdateAnimation(deltaTime);
        break;
    case Player::k_Player_State::Dead:
        //if (m_Angle <= 7200)
        //m_Angle += deltaTime * 1000;
        //m_Scale *= (1 - m_Angle / 7200);
        break;
    default:
        break;
    }

}

fw::k_ObjectType Player::GetType()
{
    return fw::k_ObjectType::Player;
}

fw::Collider* Player::GetCollider()
{
    return m_pCollider;
}

void Player::UpdateAnimation(float _deltaTime)
{

    std::string FrameName =
        k_AnimationNames[(int)m_CurrentAnimation]   // WHAT ANIMATION
        + "_"
        + k_DirectionNames[(int)m_CurrentDirection] // WHICH DIRECTION
        + "_"
        + (m_AnimationFrameNumber < 10 ? "0" : "")        // PAD THE 0s
        + std::to_string(m_AnimationFrameNumber);   // ADD FRAME NUM

    SetSprite(m_pSpriteSheet->Sprites[FrameName]);  // SET SPRITE

    if (m_CurrentAnimation != k_Animation::Run)
    {
    m_AnimationFrameTimer += k_Player_Anim_DefaultFrameRate * _deltaTime;
    }else{
    m_AnimationFrameTimer += m_RunFrameRate * _deltaTime;
    }
    m_AnimationFrameNumber = (int)m_AnimationFrameTimer % k_Player_Anim_FrameCount;

    if (m_AnimationFrameNumber == 5 && m_CurrentAnimation == k_Animation::Attack)
    {
        Vector2 DamageLocation = GetPosition() + GetDirectionVector() * 0.6f;
        m_pGameCore->GetEventManager()->AddEvent(new EnemyDamageEvent(this, DamageLocation, k_Player_AttackDamage));
    }


    if (m_AnimationFrameTimer > k_Player_Anim_FrameCount)
    {
        if (m_CurrentAnimation == k_Animation::Attack)
        {
            m_CurrentAnimation = k_Animation::Idle;
            m_CanMove = true;
        }

        m_AnimationFrameTimer = 0;
    }
    
    m_RunFrameRate = (int)(k_Player_Anim_RunFramerateBase + k_Player_Anim_RunFramerateMultiplier * m_Velocity.Magnitude());
}

Vector2 Player::GetDirectionVector()
{
    switch (m_CurrentDirection)
    {
    case Player::k_Direction::Up:
        return Vector2::Up();
        break;
    case Player::k_Direction::Down:
        return Vector2::Down();
        break;
    case Player::k_Direction::Left:
        return Vector2::Left();
        break;
    case Player::k_Direction::Right:
        return Vector2::Right();
        break;
    }
    return Vector2::Down();
}
void Player::SetSprite(fw::SpriteSheet::SpriteInfo* _spriteinfo)
{
    m_UVOffset = _spriteinfo->UV_Offset;
    m_UVScale = _spriteinfo->UV_Scale;
}

void Player::Reset()
{
}

void Player::OnCollisionHit(GameObject* _pOtherObject)
{
}

bool Player::IsInvincible()
{
    return m_InvincibilityTimer > 0;
}

void Player::Draw()
{
    GameObject::Draw();

    if(m_DrawDebugCollider)
    m_pCollider->Draw();
}

void Player::TakeDamage(int damage)
{
    if (IsInvincible())
        return;

    if (damage >= m_Health)
    {
        SetState(k_Player_State::Dead);
        m_Health = 0;
        return;
    }

    m_Health -= damage;
    m_InvincibilityTimer = k_Player_InvincibilityTime;
}

void Player::TakeDamage(fw::GameObject* instigator, int damage)
{
    m_Velocity += (GetPosition() - instigator->GetCollider()->GetRelativePosition()).Normalized() * k_Player_Kickback_Intensity;
    TakeDamage(damage);
}

void Player::OnTileCollision(fw::RectangleCollider* _tileCollider)
{
    m_Velocity.RemoveDirection((m_pCollider->GetRelativePosition() - _tileCollider->GetRelativePosition()).Normalized().SnappedToNearestAxis());
}


void Player::SetState(k_Player_State state)
{
    switch (state)
    {
    case Player::k_Player_State::Default:
        m_Color = Color::White();
        break;
    case Player::k_Player_State::Dead:
        SetColor(Color::Red());
        break;
    default:
        break;
    }
        m_CurrentState = state;
}